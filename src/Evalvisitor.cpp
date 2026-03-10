#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
    // Visit all statements in the file
    return visitChildren(ctx);
}

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    // Visit the statement (either simple_stmt or compound_stmt)
    return visitChildren(ctx);
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    // Visit simple statement
    return visitChildren(ctx);
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    // expr_stmt: testlist ( (augassign testlist) | ('=' testlist)* )
    // Get all testlists
    auto testlists = ctx->testlist();
    
    // Check for augmented assignment
    auto augassign = ctx->augassign();
    if (augassign) {
        // Augmented assignment: a += b, a -= b, etc.
        // testlists[0] is the variable, testlists[1] is the value
        if (testlists.size() >= 2) {
            // Get the variable name
            std::string varName = testlists[0]->getText();
            
            // Determine if this variable is local
            // If declared global, always use global scope
            bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
            bool isLocal = !isGlobal && (currentFunctionLocals != nullptr && 
                           currentFunctionLocals->find(varName) != currentFunctionLocals->end());
            
            // Get the current value of the variable
            Value currentValue;
            bool found = false;
            
            if (isLocal) {
                // This is a local variable - look in local scope only
                if (localVariables != nullptr) {
                    auto localIt = localVariables->find(varName);
                    if (localIt != localVariables->end()) {
                        currentValue = localIt->second;
                        found = true;
                    }
                }
            } else {
                // Check parameters (in local scope but not in assignedVars)
                if (localVariables != nullptr) {
                    auto localIt = localVariables->find(varName);
                    if (localIt != localVariables->end()) {
                        currentValue = localIt->second;
                        found = true;
                    }
                }
                // Then check global
                if (!found) {
                    auto it = variables.find(varName);
                    if (it != variables.end()) {
                        currentValue = it->second;
                        found = true;
                    }
                }
            }
            
            if (!found) {
                // Variable doesn't exist - initialize to 0
                // (In real Python, this would be UnboundLocalError for locals)
                currentValue = Value(0);
                if (isLocal && localVariables != nullptr) {
                    (*localVariables)[varName] = Value(0);
                } else {
                    variables[varName] = Value(0);
                }
            }
            
            // Evaluate the right-hand side
            auto rightAny = visit(testlists[1]);
            Value rightValue;
            if (rightAny.has_value()) {
                try {
                    rightValue = std::any_cast<Value>(rightAny);
                } catch (...) {
                    rightValue = Value(0);
                }
            } else {
                rightValue = Value(0);
            }
            
            // Get the operator
            std::string op = augassign->getText();
            
            // Apply the operation
            Value result;
            
            if (op == "+=") {
                // Addition for numbers, concatenation for strings
                if (std::holds_alternative<BigInteger>(currentValue) || std::holds_alternative<BigInteger>(rightValue)) {
                    // Promote to BigInteger if needed
                    BigInteger left = std::holds_alternative<BigInteger>(currentValue) ? 
                        std::get<BigInteger>(currentValue) : 
                        (std::holds_alternative<int>(currentValue) ? BigInteger(std::get<int>(currentValue)) : BigInteger(0));
                    BigInteger right = std::holds_alternative<BigInteger>(rightValue) ? 
                        std::get<BigInteger>(rightValue) : 
                        (std::holds_alternative<int>(rightValue) ? BigInteger(std::get<int>(rightValue)) : BigInteger(0));
                    result = left + right;
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE addition
                    if (willOverflowAdd(left, right)) {
                        result = BigInteger(left) + BigInteger(right);
                    } else {
                        result = left + right;
                    }
                } else if (std::holds_alternative<double>(currentValue) || std::holds_alternative<double>(rightValue)) {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = left + right;
                } else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<std::string>(rightValue)) {
                    result = std::get<std::string>(currentValue) + std::get<std::string>(rightValue);
                }
            } else if (op == "-=") {
                if (std::holds_alternative<BigInteger>(currentValue) || std::holds_alternative<BigInteger>(rightValue)) {
                    // Promote to BigInteger if needed
                    BigInteger left = std::holds_alternative<BigInteger>(currentValue) ? 
                        std::get<BigInteger>(currentValue) : 
                        (std::holds_alternative<int>(currentValue) ? BigInteger(std::get<int>(currentValue)) : BigInteger(0));
                    BigInteger right = std::holds_alternative<BigInteger>(rightValue) ? 
                        std::get<BigInteger>(rightValue) : 
                        (std::holds_alternative<int>(rightValue) ? BigInteger(std::get<int>(rightValue)) : BigInteger(0));
                    result = left - right;
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE subtraction
                    if (willOverflowSubtract(left, right)) {
                        result = BigInteger(left) - BigInteger(right);
                    } else {
                        result = left - right;
                    }
                } else if (std::holds_alternative<double>(currentValue) || std::holds_alternative<double>(rightValue)) {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = left - right;
                }
            } else if (op == "*=") {
                // Multiplication for numbers, repetition for string * int
                if (std::holds_alternative<BigInteger>(currentValue) || std::holds_alternative<BigInteger>(rightValue)) {
                    // Promote to BigInteger if needed
                    BigInteger left = std::holds_alternative<BigInteger>(currentValue) ? 
                        std::get<BigInteger>(currentValue) : 
                        (std::holds_alternative<int>(currentValue) ? BigInteger(std::get<int>(currentValue)) : BigInteger(0));
                    BigInteger right = std::holds_alternative<BigInteger>(rightValue) ? 
                        std::get<BigInteger>(rightValue) : 
                        (std::holds_alternative<int>(rightValue) ? BigInteger(std::get<int>(rightValue)) : BigInteger(0));
                    result = left * right;
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE multiplication
                    if (willOverflowMultiply(left, right)) {
                        result = BigInteger(left) * BigInteger(right);
                    } else {
                        result = left * right;
                    }
                } else if (std::holds_alternative<double>(currentValue) || std::holds_alternative<double>(rightValue)) {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = left * right;
                } else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    // String repetition
                    std::string s = std::get<std::string>(currentValue);
                    int count = std::get<int>(rightValue);
                    std::string repeated;
                    // Pre-allocate memory to avoid O(n²) behavior
                    repeated.reserve(s.size() * count);
                    for (int i = 0; i < count; i++) {
                        repeated.append(s);
                    }
                    result = repeated;
                }
            } else if (op == "/=") {
                // Division always returns double
                double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                result = left / right;
            } else if (op == "//=") {
                // Floor division
                if (std::holds_alternative<BigInteger>(currentValue) || std::holds_alternative<BigInteger>(rightValue)) {
                    // Promote to BigInteger if needed
                    BigInteger left = std::holds_alternative<BigInteger>(currentValue) ? 
                        std::get<BigInteger>(currentValue) : 
                        (std::holds_alternative<int>(currentValue) ? BigInteger(std::get<int>(currentValue)) : BigInteger(0));
                    BigInteger right = std::holds_alternative<BigInteger>(rightValue) ? 
                        std::get<BigInteger>(rightValue) : 
                        (std::holds_alternative<int>(rightValue) ? BigInteger(std::get<int>(rightValue)) : BigInteger(0));
                    result = left.floorDiv(right);
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE floor division
                    if (willOverflowFloorDiv(left, right)) {
                        result = BigInteger(left).floorDiv(BigInteger(right));
                    } else {
                        result = pythonFloorDiv(left, right);
                    }
                } else {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = pythonFloorDiv(static_cast<int>(left), static_cast<int>(right));
                }
            } else if (op == "%=") {
                // Modulo
                if (std::holds_alternative<BigInteger>(currentValue) || std::holds_alternative<BigInteger>(rightValue)) {
                    // Promote to BigInteger if needed
                    BigInteger left = std::holds_alternative<BigInteger>(currentValue) ? 
                        std::get<BigInteger>(currentValue) : 
                        (std::holds_alternative<int>(currentValue) ? BigInteger(std::get<int>(currentValue)) : BigInteger(0));
                    BigInteger right = std::holds_alternative<BigInteger>(rightValue) ? 
                        std::get<BigInteger>(rightValue) : 
                        (std::holds_alternative<int>(rightValue) ? BigInteger(std::get<int>(rightValue)) : BigInteger(0));
                    result = left % right;
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE modulo
                    if (willOverflowModulo(left, right)) {
                        result = BigInteger(left) % BigInteger(right);
                    } else {
                        result = pythonModulo(left, right);
                    }
                } else {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = pythonModulo(static_cast<int>(left), static_cast<int>(right));
                }
            }
            
            // Store the result (in local scope if local variable, otherwise global)
            // isLocal was already defined above
            if (isLocal && localVariables != nullptr) {
                (*localVariables)[varName] = result;
            } else if (!isLocal && localVariables != nullptr) {
                // It's a parameter or global being modified
                auto localIt = localVariables->find(varName);
                if (localIt != localVariables->end()) {
                    // Modify the parameter (local copy)
                    (*localVariables)[varName] = result;
                } else {
                    // Modify global
                    variables[varName] = result;
                }
            } else {
                variables[varName] = result;
            }
        }
        
        return std::any();
    }
    
    if (testlists.size() == 1) {
        // Just an expression, evaluate it
        return visit(testlists[0]);
    } else {
        // Assignment: a = b = c = value  OR  a, b = expr1, expr2 (tuple unpacking)
        // The last testlist is the value to assign
        // All previous testlists are variables to assign to
        
        // Get the right-hand side testlist (the values)
        auto rightTestlist = testlists.back();
        auto rightTests = rightTestlist->test();
        
        // Evaluate all values from the right-hand side
        std::vector<Value> values;
        for (auto rightTest : rightTests) {
            auto valueAny = visit(rightTest);
            Value value;
            
            if (valueAny.has_value()) {
                try {
                    value = std::any_cast<Value>(valueAny);
                } catch (...) {
                    // If not a Value, use None
                    value = Value(std::monostate{});
                }
            } else {
                value = Value(std::monostate{});
            }
            values.push_back(value);
        }
        
        // Assign to all variables on the left (excluding the last testlist which is the RHS)
        for (size_t i = 0; i < testlists.size() - 1; i++) {
            auto testlist = testlists[i];
            auto tests = testlist->test();
            
            // Check if this is tuple unpacking (multiple tests on the left)
            if (tests.size() > 1) {
                // Tuple unpacking: a, b = expr1, expr2
                // Assign each value to the corresponding variable
                for (size_t j = 0; j < tests.size(); j++) {
                    std::string varName = tests[j]->getText();
                    Value value = (j < values.size()) ? values[j] : Value(std::monostate{});
                    
                    // Determine if this variable is local
                    // If declared global, always use global scope
                    bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
                    bool isLocal = !isGlobal && (currentFunctionLocals != nullptr && 
                                   currentFunctionLocals->find(varName) != currentFunctionLocals->end());
                    
                    // Assign to the appropriate scope
                    if (isLocal && localVariables != nullptr) {
                        (*localVariables)[varName] = value;
                    } else {
                        variables[varName] = value;
                    }
                }
            } else if (!tests.empty()) {
                // Simple assignment: a = value (or a = b = c = value)
                std::string varName = tests[0]->getText();
                Value value;
                
                if (values.size() == 1) {
                    value = values[0];
                } else {
                    // If RHS has multiple values but LHS is single, assign the first value
                    value = !values.empty() ? values[0] : Value(std::monostate{});
                }
                
                // Determine if this variable is local
                // If declared global, always use global scope
                bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
                bool isLocal = !isGlobal && (currentFunctionLocals != nullptr && 
                               currentFunctionLocals->find(varName) != currentFunctionLocals->end());
                
                // Assign to the appropriate scope
                if (isLocal && localVariables != nullptr) {
                    (*localVariables)[varName] = value;
                } else {
                    variables[varName] = value;
                }
            }
        }
        
        return std::any();
    }
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    // atom_expr: atom trailer?
    // Get the atom (function name or value)
    auto atom = ctx->atom();
    if (!atom) {
        return std::any();
    }
    
    // Check if this is a function call (has trailer)
    auto trailer = ctx->trailer();
    if (trailer) {
        // Get the function name from the atom
        std::string funcName = atom->getText();
        
        // Handle print function
        if (funcName == "print") {
            // Get the arglist from the trailer
            auto arglist = trailer->arglist();
            if (arglist) {
                // Get the arguments
                auto args = arglist->argument();
                if (!args.empty()) {
                    // Loop through all arguments and print them space-separated
                    for (size_t i = 0; i < args.size(); ++i) {
                        auto arg = args[i];
                        auto tests = arg->test();
                        if (!tests.empty()) {
                            // Visit the test to get its value
                            auto argValue = visit(tests[0]);
                            
                            // If it's a Value, print it
                            if (argValue.has_value()) {
                                try {
                                    Value val = std::any_cast<Value>(argValue);
                                    std::cout << valueToString(val);
                                    // Print space after each argument except the last
                                    if (i < args.size() - 1) {
                                        std::cout << " ";
                                    }
                                } catch (...) {
                                    // Not a Value, ignore
                                }
                            }
                        }
                    }
                }
            }
            // Always print newline, even if no arguments
            std::cout << std::endl;
            return std::any();
        }
        
        // Handle int() type conversion function
        if (funcName == "int") {
            // Get the argument
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty() && args.size() == 1) {
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                
                                // Convert to int based on the type
                                if (std::holds_alternative<int>(val)) {
                                    // Already an int
                                    return val;
                                } else if (std::holds_alternative<double>(val)) {
                                    // float → int: truncate decimal part
                                    int intVal = static_cast<int>(std::get<double>(val));
                                    return Value(intVal);
                                } else if (std::holds_alternative<bool>(val)) {
                                    // bool → int: True → 1, False → 0
                                    return Value(std::get<bool>(val) ? 1 : 0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    // str → int: parse string as integer
                                    std::string str = std::get<std::string>(val);
                                    // Check if it's a large number that needs BigInteger
                                    bool negative = !str.empty() && str[0] == '-';
                                    std::string absStr = negative ? str.substr(1) : str;
                                    if (absStr.length() > 10 || 
                                        (absStr.length() == 10 && absStr > "2147483647")) {
                                        // Use BigInteger for large integers
                                        return Value(BigInteger(str));
                                    } else {
                                        // Use regular int
                                        return Value(std::stoi(str));
                                    }
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    // BigInteger → keep as BigInteger
                                    return val;
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            // Return 0 if conversion failed
            return Value(0);
        }
        
        // Handle float() type conversion function
        if (funcName == "float") {
            // Get the argument
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty() && args.size() == 1) {
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                
                                // Convert to float based on the type
                                if (std::holds_alternative<double>(val)) {
                                    // Already a float
                                    return val;
                                } else if (std::holds_alternative<int>(val)) {
                                    // int → float: direct conversion
                                    return Value(static_cast<double>(std::get<int>(val)));
                                } else if (std::holds_alternative<bool>(val)) {
                                    // bool → float: True → 1.0, False → 0.0
                                    return Value(std::get<bool>(val) ? 1.0 : 0.0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    // str → float: parse string as float
                                    std::string str = std::get<std::string>(val);
                                    return Value(std::stod(str));
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    // BigInteger → float: convert to double
                                    BigInteger bi = std::get<BigInteger>(val);
                                    // Try to convert to long long first, then to double
                                    try {
                                        long long ll = bi.toLongLong();
                                        return Value(static_cast<double>(ll));
                                    } catch (...) {
                                        // If too large for long long, parse the string representation
                                        return Value(std::stod(bi.toString()));
                                    }
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            // Return 0.0 if conversion failed
            return Value(0.0);
        }
        
        // Handle str() type conversion function
        if (funcName == "str") {
            // Get the argument
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty() && args.size() == 1) {
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                
                                // Convert to str based on the type
                                if (std::holds_alternative<std::string>(val)) {
                                    // Already a string
                                    return val;
                                } else if (std::holds_alternative<int>(val)) {
                                    // int → str: convert to string
                                    return Value(std::to_string(std::get<int>(val)));
                                } else if (std::holds_alternative<double>(val)) {
                                    // float → str: format with 6 decimal places, remove trailing zeros
                                    double d = std::get<double>(val);
                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(6) << d;
                                    std::string result = oss.str();
                                    // Remove trailing zeros after decimal point
                                    size_t dotPos = result.find('.');
                                    if (dotPos != std::string::npos) {
                                        size_t lastNonZero = result.find_last_not_of('0');
                                        if (lastNonZero > dotPos) {
                                            result = result.substr(0, lastNonZero + 1);
                                        } else {
                                            // Keep at least one decimal place
                                            result = result.substr(0, dotPos + 2);
                                        }
                                    }
                                    return Value(result);
                                } else if (std::holds_alternative<bool>(val)) {
                                    // bool → str: "True" or "False"
                                    return Value(std::get<bool>(val) ? "True" : "False");
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    // BigInteger → str: toString()
                                    return Value(std::get<BigInteger>(val).toString());
                                } else if (std::holds_alternative<std::monostate>(val)) {
                                    // None → str: "None"
                                    return Value("None");
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            // Return empty string if conversion failed
            return Value("");
        }
        
        // Handle bool() type conversion function
        if (funcName == "bool") {
            // Get the argument
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty() && args.size() == 1) {
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                
                                // Convert to bool based on the type
                                if (std::holds_alternative<bool>(val)) {
                                    // Already a bool
                                    return val;
                                } else if (std::holds_alternative<int>(val)) {
                                    // int → bool: 0 → False, non-zero → True
                                    return Value(std::get<int>(val) != 0);
                                } else if (std::holds_alternative<double>(val)) {
                                    // float → bool: 0.0 → False, non-zero → True
                                    return Value(std::get<double>(val) != 0.0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    // str → bool: "" → False, non-empty → True
                                    return Value(!std::get<std::string>(val).empty());
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    // BigInteger → bool: 0 → False, non-zero → True
                                    return Value(!std::get<BigInteger>(val).isZero());
                                } else if (std::holds_alternative<std::monostate>(val)) {
                                    // None → bool: False
                                    return Value(false);
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            // Return False if conversion failed
            return Value(false);
        }
        
        // Check if this is a user-defined function
        auto funcIt = functions.find(funcName);
        if (funcIt != functions.end()) {
            // This is a user-defined function call
            const FunctionDef& funcDef = funcIt->second;
            
            // Evaluate the arguments
            std::vector<Value> argValues;
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                for (auto arg : args) {
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                argValues.push_back(val);
                            } catch (...) {
                                argValues.push_back(Value(std::monostate{}));
                            }
                        } else {
                            argValues.push_back(Value(std::monostate{}));
                        }
                    }
                }
            }
            
            // Create local variable scope
            std::map<std::string, Value> localVars;
            std::map<std::string, Value>* savedLocalVariables = localVariables;
            const std::set<std::string>* savedFunctionLocals = currentFunctionLocals;
            std::set<std::string> savedFunctionGlobals = currentFunctionGlobals;
            
            localVariables = &localVars;
            currentFunctionLocals = &funcDef.assignedVars;
            currentFunctionGlobals = funcDef.globalVars;
            
            // Bind parameters to arguments (parameters are always local)
            for (size_t i = 0; i < funcDef.parameters.size() && i < argValues.size(); i++) {
                localVars[funcDef.parameters[i]] = argValues[i];
            }
            
            // Execute the function body
            Value returnValue = Value(std::monostate{});  // Default return value is None
            try {
                visit(funcDef.body);
            } catch (const ReturnException& e) {
                // Function returned a value
                returnValue = e.returnValue;
            }
            
            // Restore previous scope
            localVariables = savedLocalVariables;
            currentFunctionLocals = savedFunctionLocals;
            currentFunctionGlobals = savedFunctionGlobals;
            
            // Return the value
            return returnValue;
        }
        
        return std::any();
    } else {
        // No trailer - this is just an atom expression, visit the atom
        return visit(atom);
    }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Check if this is a format string (f-string)
    auto format_string = ctx->format_string();
    if (format_string) {
        return visit(format_string);
    }
    
    // Check if this is a string literal
    auto strings = ctx->STRING();
    if (!strings.empty()) {
        // Get the string literal (may have quotes)
        std::string strValue = strings[0]->getText();
        // Remove quotes
        strValue = unquoteString(strValue);
        // Return as Value
        return Value(strValue);
    }
    
    // Check if this is a number
    auto number = ctx->NUMBER();
    if (number) {
        std::string numStr = number->getText();
        // Check if it's a float (contains decimal point or scientific notation)
        if (numStr.find('.') != std::string::npos || 
            numStr.find('e') != std::string::npos || 
            numStr.find('E') != std::string::npos) {
            // Parse as float
            double numValue = std::stod(numStr);
            return Value(numValue);
        } else {
            // Parse as integer - check if it fits in int
            // int can hold roughly -2B to +2B, which is 10 digits for positive
            // If the number is too large, use BigInteger
            bool negative = !numStr.empty() && numStr[0] == '-';
            std::string absNumStr = negative ? numStr.substr(1) : numStr;
            
            // Check if number is too large for int (more than 10 digits, or exactly 10 and >= 2147483648)
            if (absNumStr.length() > 10 || 
                (absNumStr.length() == 10 && absNumStr > "2147483647")) {
                // Use BigInteger for large integers
                return Value(BigInteger(numStr));
            } else {
                // Use regular int
                int numValue = std::stoi(numStr);
                return Value(numValue);
            }
        }
    }
    
    // Check if this is True
    if (ctx->getText() == "True") {
        return Value(true);
    }
    
    // Check if this is False
    if (ctx->getText() == "False") {
        return Value(false);
    }
    
    // Check if this is None
    if (ctx->getText() == "None") {
        return Value(std::monostate{});
    }
    
    // Check if this is a NAME (variable reference)
    auto name = ctx->NAME();
    if (name) {
        std::string varName = name->getText();
        // Check if declared global
        bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
        
        // If we're in a function and this variable is local (and NOT global), look in local scope
        if (!isGlobal && currentFunctionLocals != nullptr && currentFunctionLocals->find(varName) != currentFunctionLocals->end()) {
            // This is a local variable - must look in local scope only
            if (localVariables != nullptr) {
                auto localIt = localVariables->find(varName);
                if (localIt != localVariables->end()) {
                    return localIt->second;
                }
            }
            // Local variable not initialized - return None (or should error)
            return Value(std::monostate{});
        }
        // Otherwise, check global variables (includes parameters which are in local scope but not in assignedVars, and globals)
        if (!isGlobal && localVariables != nullptr) {
            auto localIt = localVariables->find(varName);
            if (localIt != localVariables->end()) {
                return localIt->second;
            }
        }
        auto it = variables.find(varName);
        if (it != variables.end()) {
            return it->second;
        } else {
            // Variable not found, return None
            return Value(std::monostate{});
        }
    }
    
    // Check if this is a parenthesized test expression: '(' test ')'
    auto test = ctx->test();
    if (test) {
        return visit(test);
    }
    
    // For other atoms, return empty
    return std::any();
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    // Trailer is handled in visitAtom_expr
    return visitChildren(ctx);
}

std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
    // f-string format: f"text {expr} more text {expr2} ..."
    // Children are:
    // - FORMAT_QUOTATION (f" or f')
    // - Alternating FORMAT_STRING_LITERAL (static text) and testlist (expressions in {})
    // - QUOTATION (closing " or ')
    
    std::string result;
    
    // Get all children and process them
    auto children = ctx->children;
    
    for (size_t i = 0; i < children.size(); i++) {
        auto child = children[i];
        
        // Skip the opening FORMAT_QUOTATION (f" or f')
        if (auto terminal = dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            if (terminal->getSymbol()->getType() == Python3Parser::FORMAT_QUOTATION) {
                continue;
            }
            // Skip the closing QUOTATION (" or ')
            if (terminal->getSymbol()->getType() == Python3Parser::QUOTATION) {
                continue;
            }
            // Process FORMAT_STRING_LITERAL (static text)
            if (terminal->getSymbol()->getType() == Python3Parser::FORMAT_STRING_LITERAL) {
                std::string text = terminal->getText();
                // Handle escaped braces: {{ -> {, }} -> }
                std::string processed;
                for (size_t j = 0; j < text.length(); j++) {
                    if (j + 1 < text.length() && text[j] == '{' && text[j + 1] == '{') {
                        processed += '{';
                        j++; // Skip next character
                    } else if (j + 1 < text.length() && text[j] == '}' && text[j + 1] == '}') {
                        processed += '}';
                        j++; // Skip next character
                    } else {
                        processed += text[j];
                    }
                }
                result += processed;
                continue;
            }
            // Skip OPEN_BRACE - it's handled by the testlist
            if (terminal->getSymbol()->getType() == Python3Parser::OPEN_BRACE) {
                continue;
            }
            // Skip CLOSE_BRACE - it's handled by the testlist
            if (terminal->getSymbol()->getType() == Python3Parser::CLOSE_BRACE) {
                continue;
            }
        }
        
        // Process testlist (expressions in {})
        if (auto testlist = dynamic_cast<Python3Parser::TestlistContext*>(child)) {
            // Evaluate the expression
            auto exprValue = visit(testlist);
            if (exprValue.has_value()) {
                try {
                    Value val = std::any_cast<Value>(exprValue);
                    // Convert the value to string and append
                    result += valueToString(val);
                } catch (...) {
                    // If conversion fails, skip
                }
            }
        }
    }
    
    return Value(result);
}

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    // arith_expr: term (addorsub_op term)*
    auto terms = ctx->term();
    if (terms.empty()) {
        return std::any();
    }
    
    // Visit the first term
    auto resultAny = visit(terms[0]);
    if (!resultAny.has_value()) {
        return Value(0);
    }
    
    Value result;
    try {
        result = std::any_cast<Value>(resultAny);
    } catch (...) {
        return Value(0);
    }
    
    // Process remaining terms with operators
    auto ops = ctx->addorsub_op();
    for (size_t i = 0; i < ops.size(); i++) {
        auto termValue = visit(terms[i + 1]);
        if (!termValue.has_value()) {
            continue;
        }
        
        Value term;
        try {
            term = std::any_cast<Value>(termValue);
        } catch (...) {
            continue;
        }
        
        std::string op = ops[i]->getText();
        
        // Type coercion and operation
        // BigInteger has highest priority, then double, then int
        if (std::holds_alternative<BigInteger>(result) || std::holds_alternative<BigInteger>(term)) {
            // Promote to BigInteger if needed
            BigInteger left = std::holds_alternative<BigInteger>(result) ? 
                std::get<BigInteger>(result) : 
                (std::holds_alternative<int>(result) ? BigInteger(std::get<int>(result)) : BigInteger(0));
            BigInteger right = std::holds_alternative<BigInteger>(term) ? 
                std::get<BigInteger>(term) : 
                (std::holds_alternative<int>(term) ? BigInteger(std::get<int>(term)) : BigInteger(0));
            if (op == "+") {
                result = left + right;
            } else if (op == "-") {
                result = left - right;
            }
        } else if (std::holds_alternative<int>(result) && std::holds_alternative<int>(term)) {
            // int op int -> int
            int left = std::get<int>(result);
            int right = std::get<int>(term);
            if (op == "+") {
                // Check for overflow BEFORE addition
                if (willOverflowAdd(left, right)) {
                    // Promote to BigInteger to handle large result
                    result = BigInteger(left) + BigInteger(right);
                } else {
                    result = left + right;
                }
            } else if (op == "-") {
                // Check for overflow BEFORE subtraction
                if (willOverflowSubtract(left, right)) {
                    // Promote to BigInteger to handle large result
                    result = BigInteger(left) - BigInteger(right);
                } else {
                    result = left - right;
                }
            }
        } else if (std::holds_alternative<double>(result) || std::holds_alternative<double>(term)) {
            // int op double -> double OR double op int -> double OR double op double -> double
            double left = std::holds_alternative<double>(result) ? std::get<double>(result) : static_cast<double>(std::get<int>(result));
            double right = std::holds_alternative<double>(term) ? std::get<double>(term) : static_cast<double>(std::get<int>(term));
            if (op == "+") {
                result = left + right;
            } else if (op == "-") {
                result = left - right;
            }
        } else if (std::holds_alternative<std::string>(result) && std::holds_alternative<std::string>(term)) {
            // string op string
            if (op == "+") {
                // String concatenation
                result = std::get<std::string>(result) + std::get<std::string>(term);
            }
        }
    }
    
    return result;
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    // term: factor (muldivmod_op factor)*
    auto factors = ctx->factor();
    if (factors.empty()) {
        return std::any();
    }
    
    // Visit the first factor
    auto resultAny = visit(factors[0]);
    if (!resultAny.has_value()) {
        return Value(0);
    }
    
    Value result;
    try {
        result = std::any_cast<Value>(resultAny);
    } catch (...) {
        return Value(0);
    }
    
    // Process remaining factors with operators
    auto ops = ctx->muldivmod_op();
    for (size_t i = 0; i < ops.size(); i++) {
        auto factorValue = visit(factors[i + 1]);
        if (!factorValue.has_value()) {
            continue;
        }
        
        Value factor;
        try {
            factor = std::any_cast<Value>(factorValue);
        } catch (...) {
            continue;
        }
        
        std::string op = ops[i]->getText();
        
        // Type coercion and operation
        // Handle string multiplication: string * int or int * string
        if (op == "*" && ((std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) ||
                          (std::holds_alternative<int>(result) && std::holds_alternative<std::string>(factor)))) {
            std::string s;
            int count;
            
            if (std::holds_alternative<std::string>(result)) {
                // string * int
                s = std::get<std::string>(result);
                count = std::get<int>(factor);
            } else {
                // int * string
                count = std::get<int>(result);
                s = std::get<std::string>(factor);
            }
            
            // Handle edge cases
            if (count <= 0) {
                result = std::string("");
            } else {
                std::string repeated;
                // Pre-allocate memory to avoid O(n²) behavior
                repeated.reserve(s.size() * count);
                for (int i = 0; i < count; i++) {
                    repeated.append(s);
                }
                result = repeated;
            }
        } else if (std::holds_alternative<BigInteger>(result) || std::holds_alternative<BigInteger>(factor)) {
            // Handle BigInteger operations
            // Promote to BigInteger if needed
            BigInteger left = std::holds_alternative<BigInteger>(result) ? 
                std::get<BigInteger>(result) : 
                (std::holds_alternative<int>(result) ? BigInteger(std::get<int>(result)) : BigInteger(0));
            BigInteger right = std::holds_alternative<BigInteger>(factor) ? 
                std::get<BigInteger>(factor) : 
                (std::holds_alternative<int>(factor) ? BigInteger(std::get<int>(factor)) : BigInteger(0));
            if (op == "*") {
                result = left * right;
            } else if (op == "//") {
                result = left.floorDiv(right);
            } else if (op == "%") {
                result = left % right;
            } else if (op == "/") {
                // BigInteger division returns double
                // Convert to string and then to double for precision
                double leftD = std::stod(left.toString());
                double rightD = std::stod(right.toString());
                result = leftD / rightD;
            }
        } else if (op == "/") {
            // Division always returns double
            double left = std::holds_alternative<double>(result) ? std::get<double>(result) : static_cast<double>(std::get<int>(result));
            double right = std::holds_alternative<double>(factor) ? std::get<double>(factor) : static_cast<double>(std::get<int>(factor));
            result = left / right;
        } else if (std::holds_alternative<int>(result) && std::holds_alternative<int>(factor)) {
            // int op int -> int (except /)
            int left = std::get<int>(result);
            int right = std::get<int>(factor);
            if (op == "*") {
                // Check for overflow BEFORE multiplication
                if (willOverflowMultiply(left, right)) {
                    // Promote to BigInteger to handle large result
                    result = BigInteger(left) * BigInteger(right);
                } else {
                    result = left * right;
                }
            } else if (op == "//") {
                // Check for overflow BEFORE floor division
                if (willOverflowFloorDiv(left, right)) {
                    // Promote to BigInteger
                    result = BigInteger(left).floorDiv(BigInteger(right));
                } else {
                    result = pythonFloorDiv(left, right);
                }
            } else if (op == "%") {
                // Check for overflow BEFORE modulo (though it rarely overflows)
                if (willOverflowModulo(left, right)) {
                    // Promote to BigInteger
                    result = BigInteger(left) % BigInteger(right);
                } else {
                    result = pythonModulo(left, right);
                }
            }
        } else {
            // int op double -> double OR double op int -> double OR double op double -> double
            double left = std::holds_alternative<double>(result) ? std::get<double>(result) : static_cast<double>(std::get<int>(result));
            double right = std::holds_alternative<double>(factor) ? std::get<double>(factor) : static_cast<double>(std::get<int>(factor));
            if (op == "*") {
                result = left * right;
            } else if (op == "//") {
                result = pythonFloorDiv(static_cast<int>(left), static_cast<int>(right));
            } else if (op == "%") {
                result = pythonModulo(static_cast<int>(left), static_cast<int>(right));
            }
        }
    }
    
    return result;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    // factor: ('+'|'-') factor | atom_expr
    
    // Check if there's a unary operator
    auto text = ctx->getText();
    if (text.length() > 0 && (text[0] == '+' || text[0] == '-')) {
        // This is a unary expression
        auto factor = ctx->factor();
        if (factor) {
            auto factorValue = visit(factor);
            if (!factorValue.has_value()) {
                return Value(0);
            }
            
            Value factorVal;
            try {
                factorVal = std::any_cast<Value>(factorValue);
            } catch (...) {
                return Value(0);
            }
            
            if (text[0] == '-') {
                // Unary negation
                if (std::holds_alternative<int>(factorVal)) {
                    int value = std::get<int>(factorVal);
                    // Check for INT_MIN overflow - promote to BigInteger
                    if (value == INT_MIN) {
                        BigInteger bigVal(value);
                        return Value(-bigVal);
                    }
                    return Value(-value);
                } else if (std::holds_alternative<double>(factorVal)) {
                    return Value(-std::get<double>(factorVal));
                } else if (std::holds_alternative<BigInteger>(factorVal)) {
                    return Value(-std::get<BigInteger>(factorVal));
                }
            } else {
                // Unary plus (no-op)
                return factorVal;
            }
        }
    } else {
        // This is just an atom_expr
        auto atomExpr = ctx->atom_expr();
        if (atomExpr) {
            return visit(atomExpr);
        }
    }
    
    return std::any();
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    // comparison: arith_expr (comp_op arith_expr)*
    auto arithExprs = ctx->arith_expr();
    if (arithExprs.empty()) {
        return std::any();
    }
    
    // If there's only one arith_expr, just return its value (no comparison)
    if (arithExprs.size() == 1) {
        return visit(arithExprs[0]);
    }
    
    // Visit the first arith_expr
    auto leftAny = visit(arithExprs[0]);
    if (!leftAny.has_value()) {
        return Value(false);
    }
    
    Value left;
    try {
        left = std::any_cast<Value>(leftAny);
    } catch (...) {
        return Value(false);
    }
    
    // Process all comparison operations (Python supports chaining: a < b < c)
    auto ops = ctx->comp_op();
    bool finalResult = true;
    
    for (size_t i = 0; i < ops.size(); i++) {
        auto rightAny = visit(arithExprs[i + 1]);
        if (!rightAny.has_value()) {
            return Value(false);
        }
        
        Value right;
        try {
            right = std::any_cast<Value>(rightAny);
        } catch (...) {
            return Value(false);
        }
        
        std::string op = ops[i]->getText();
        bool compResult = false;
        
        // Perform comparison based on types
        // Handle BigInteger comparisons
        if (std::holds_alternative<BigInteger>(left) || std::holds_alternative<BigInteger>(right)) {
            // Promote to BigInteger if needed
            BigInteger l = std::holds_alternative<BigInteger>(left) ? 
                std::get<BigInteger>(left) : 
                (std::holds_alternative<int>(left) ? BigInteger(std::get<int>(left)) : BigInteger(0));
            BigInteger r = std::holds_alternative<BigInteger>(right) ? 
                std::get<BigInteger>(right) : 
                (std::holds_alternative<int>(right) ? BigInteger(std::get<int>(right)) : BigInteger(0));
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            // int vs int
            int l = std::get<int>(left);
            int r = std::get<int>(right);
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if ((std::holds_alternative<int>(left) || std::holds_alternative<double>(left)) &&
                   (std::holds_alternative<int>(right) || std::holds_alternative<double>(right))) {
            // int vs double OR double vs int OR double vs double
            double l = std::holds_alternative<double>(left) ? std::get<double>(left) : static_cast<double>(std::get<int>(left));
            double r = std::holds_alternative<double>(right) ? std::get<double>(right) : static_cast<double>(std::get<int>(right));
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
            // string vs string (lexicographic comparison)
            std::string l = std::get<std::string>(left);
            std::string r = std::get<std::string>(right);
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
            // bool vs bool
            bool l = std::get<bool>(left);
            bool r = std::get<bool>(right);
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else {
            // Different types or None - for now, just return false
            // In Python, different types have specific comparison rules
            compResult = false;
        }
        
        finalResult = finalResult && compResult;
        
        // For chained comparisons, the right becomes the new left
        left = right;
    }
    
    return Value(finalResult);
}

std::string EvalVisitor::unquoteString(const std::string& str) {
    // Remove surrounding quotes from string literals
    if (str.length() >= 2) {
        if ((str.front() == '"' && str.back() == '"') ||
            (str.front() == '\'' && str.back() == '\'')) {
            return str.substr(1, str.length() - 2);
        }
    }
    return str;
}

std::string EvalVisitor::valueToString(const Value& val) {
    // Convert a Value to its string representation
    // This is used for both print() and f-string interpolation
    if (std::holds_alternative<std::string>(val)) {
        return std::get<std::string>(val);
    } else if (std::holds_alternative<int>(val)) {
        return std::to_string(std::get<int>(val));
    } else if (std::holds_alternative<double>(val)) {
        // Python always shows at least one decimal place for floats
        double d = std::get<double>(val);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << d;
        std::string result = oss.str();
        // Remove trailing zeros after decimal point
        size_t dotPos = result.find('.');
        if (dotPos != std::string::npos) {
            size_t lastNonZero = result.find_last_not_of('0');
            if (lastNonZero > dotPos) {
                result = result.substr(0, lastNonZero + 1);
            } else {
                // Keep at least one decimal place
                result = result.substr(0, dotPos + 2);
            }
        }
        return result;
    } else if (std::holds_alternative<bool>(val)) {
        // Python prints True/False, not 1/0
        return std::get<bool>(val) ? "True" : "False";
    } else if (std::holds_alternative<std::monostate>(val)) {
        // Python prints None
        return "None";
    } else if (std::holds_alternative<BigInteger>(val)) {
        // Print BigInteger
        return std::get<BigInteger>(val).toString();
    }
    return "";
}

bool EvalVisitor::valueToBool(const Value& val) {
    // Convert Python value to bool using Python's truthiness rules
    // False values: None, False, 0, 0.0, empty string "", BigInteger(0)
    // True values: everything else
    if (std::holds_alternative<std::monostate>(val)) {
        return false; // None is falsy
    } else if (std::holds_alternative<bool>(val)) {
        return std::get<bool>(val);
    } else if (std::holds_alternative<int>(val)) {
        return std::get<int>(val) != 0;
    } else if (std::holds_alternative<double>(val)) {
        return std::get<double>(val) != 0.0;
    } else if (std::holds_alternative<std::string>(val)) {
        return !std::get<std::string>(val).empty();
    } else if (std::holds_alternative<BigInteger>(val)) {
        return !std::get<BigInteger>(val).isZero();
    }
    return false;
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    // compound_stmt: if_stmt | while_stmt | funcdef
    // Route to the appropriate handler
    return visitChildren(ctx);
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    // while_stmt: 'while' test ':' suite
    auto test = ctx->test();
    auto suite = ctx->suite();
    
    if (!test || !suite) {
        return std::any();
    }
    
    // Loop while the test condition is true
    while (true) {
        // Evaluate the condition
        auto conditionAny = visit(test);
        if (!conditionAny.has_value()) {
            break;
        }
        
        Value conditionValue;
        try {
            conditionValue = std::any_cast<Value>(conditionAny);
        } catch (...) {
            break;
        }
        
        // Check if condition is true
        if (!valueToBool(conditionValue)) {
            break;
        }
        
        // Execute the suite (body of the while loop)
        try {
            visit(suite);
        } catch (const BreakException&) {
            // Break out of the loop
            break;
        } catch (const ContinueException&) {
            // Continue to next iteration
            continue;
        }
    }
    
    return std::any();
}

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    // if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ('else' ':' suite)?
    auto tests = ctx->test();
    auto suites = ctx->suite();
    
    if (tests.empty() || suites.empty()) {
        return std::any();
    }
    
    // Evaluate the if condition
    auto conditionAny = visit(tests[0]);
    if (conditionAny.has_value()) {
        Value conditionValue;
        try {
            conditionValue = std::any_cast<Value>(conditionAny);
        } catch (...) {
            return std::any();
        }
        
        if (valueToBool(conditionValue)) {
            // Execute the if suite
            visit(suites[0]);
            return std::any();
        }
    }
    
    // Check elif conditions
    for (size_t i = 1; i < tests.size(); i++) {
        auto elifConditionAny = visit(tests[i]);
        if (elifConditionAny.has_value()) {
            Value elifConditionValue;
            try {
                elifConditionValue = std::any_cast<Value>(elifConditionAny);
            } catch (...) {
                continue;
            }
            
            if (valueToBool(elifConditionValue)) {
                // Execute the elif suite
                visit(suites[i]);
                return std::any();
            }
        }
    }
    
    // If there's an else clause (more suites than tests), execute it
    if (suites.size() > tests.size()) {
        visit(suites.back());
    }
    
    return std::any();
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    // suite: simple_stmt | NEWLINE INDENT stmt+ DEDENT
    // Visit all statements in the suite
    return visitChildren(ctx);
}

std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    // test: or_test
    auto orTest = ctx->or_test();
    if (orTest) {
        return visit(orTest);
    }
    return std::any();
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    // or_test: and_test ('or' and_test)*
    auto andTests = ctx->and_test();
    if (andTests.empty()) {
        return std::any();
    }
    
    // Evaluate first and_test
    auto resultAny = visit(andTests[0]);
    if (!resultAny.has_value()) {
        return Value(false);
    }
    
    Value result;
    try {
        result = std::any_cast<Value>(resultAny);
    } catch (...) {
        return Value(false);
    }
    
    // If there's only one and_test, return it
    if (andTests.size() == 1) {
        return result;
    }
    
    // Short-circuit evaluation: if first is true, return it
    if (valueToBool(result)) {
        return result;
    }
    
    // Otherwise, evaluate remaining and_tests
    for (size_t i = 1; i < andTests.size(); i++) {
        auto andTestAny = visit(andTests[i]);
        if (andTestAny.has_value()) {
            try {
                result = std::any_cast<Value>(andTestAny);
                if (valueToBool(result)) {
                    return result;
                }
            } catch (...) {
                // Continue to next and_test
            }
        }
    }
    
    return result;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    // and_test: not_test ('and' not_test)*
    auto notTests = ctx->not_test();
    if (notTests.empty()) {
        return std::any();
    }
    
    // Evaluate first not_test
    auto resultAny = visit(notTests[0]);
    if (!resultAny.has_value()) {
        return Value(false);
    }
    
    Value result;
    try {
        result = std::any_cast<Value>(resultAny);
    } catch (...) {
        return Value(false);
    }
    
    // If there's only one not_test, return it
    if (notTests.size() == 1) {
        return result;
    }
    
    // Short-circuit evaluation: if first is false, return it
    if (!valueToBool(result)) {
        return result;
    }
    
    // Otherwise, evaluate remaining not_tests
    for (size_t i = 1; i < notTests.size(); i++) {
        auto notTestAny = visit(notTests[i]);
        if (notTestAny.has_value()) {
            try {
                result = std::any_cast<Value>(notTestAny);
                if (!valueToBool(result)) {
                    return result;
                }
            } catch (...) {
                return Value(false);
            }
        }
    }
    
    return result;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    // not_test: 'not' not_test | comparison
    
    // Check if this has a 'not' operator
    auto notTest = ctx->not_test();
    if (notTest) {
        // This is 'not' not_test
        auto notTestAny = visit(notTest);
        if (!notTestAny.has_value()) {
            return Value(true); // not None = True
        }
        
        Value notTestValue;
        try {
            notTestValue = std::any_cast<Value>(notTestAny);
        } catch (...) {
            return Value(true);
        }
        
        // Return the negation
        return Value(!valueToBool(notTestValue));
    }
    
    // Otherwise, this is just a comparison
    auto comparison = ctx->comparison();
    if (comparison) {
        return visit(comparison);
    }
    
    return std::any();
}

std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    // funcdef: 'def' NAME parameters ':' suite
    // Get the function name
    auto nameToken = ctx->NAME();
    if (!nameToken) {
        return std::any();
    }
    std::string funcName = nameToken->getText();
    
    // Get the parameters
    auto parametersCtx = ctx->parameters();
    std::vector<std::string> params;
    
    if (parametersCtx) {
        // parameters: '(' typedargslist? ')'
        auto typedargslist = parametersCtx->typedargslist();
        if (typedargslist) {
            // typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
            // tfpdef: NAME
            auto tfpdefs = typedargslist->tfpdef();
            for (auto tfpdef : tfpdefs) {
                auto paramName = tfpdef->NAME();
                if (paramName) {
                    params.push_back(paramName->getText());
                }
            }
        }
    }
    
    // Get the function body (suite)
    auto suite = ctx->suite();
    
    // Find all variables assigned in the function body
    std::set<std::string> assignedVars;
    findAssignedVariables(suite, assignedVars);
    
    // Find all global declarations in the function body
    std::set<std::string> globalVars;
    findGlobalDeclarations(suite, globalVars);
    
    // Store the function definition
    FunctionDef funcDef;
    funcDef.parameters = params;
    funcDef.body = suite;
    funcDef.assignedVars = assignedVars;
    funcDef.globalVars = globalVars;
    functions[funcName] = funcDef;
    
    return std::any();
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    // return_stmt: 'return' [testlist]
    // Evaluate the return expression (if present)
    Value returnValue = Value(std::monostate{});  // Default to None
    
    auto testlist = ctx->testlist();
    if (testlist) {
        // testlist can have multiple tests, but typically just one for return
        auto tests = testlist->test();
        if (!tests.empty()) {
            auto result = visit(tests[0]);
            if (result.has_value()) {
                try {
                    returnValue = std::any_cast<Value>(result);
                } catch (...) {
                    // If cast fails, return None
                    returnValue = Value(std::monostate{});
                }
            }
        }
    }
    
    // Throw the exception to exit the function
    throw ReturnException(returnValue);
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
    // break_stmt: 'break'
    // Throw exception to exit the loop
    throw BreakException();
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
    // continue_stmt: 'continue'
    // Throw exception to skip to next iteration
    throw ContinueException();
}

std::any EvalVisitor::visitGlobal_stmt(Python3Parser::Global_stmtContext *ctx) {
    // global_stmt: GLOBAL NAME (',' NAME)*
    // The global declarations were already collected during function definition parsing.
    // This visitor method is here to handle global statements during execution (they're no-ops at runtime).
    return nullptr;
}

// Python-style floor division: floors toward -∞
int EvalVisitor::pythonFloorDiv(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }
    
    // C++ division truncates toward zero
    // Python floor division floors toward -∞
    int q = a / b;
    int r = a % b;
    
    // If signs differ and there's a remainder, adjust quotient down by 1
    if ((a < 0) != (b < 0) && r != 0) {
        q -= 1;
    }
    
    return q;
}

// Python-style modulo: result has same sign as divisor
int EvalVisitor::pythonModulo(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Modulo by zero");
    }
    
    // C++ modulo has same sign as dividend
    // Python modulo has same sign as divisor
    int r = a % b;
    
    // If signs differ and remainder is non-zero, adjust
    if ((a < 0) != (b < 0) && r != 0) {
        r += b;
    }
    
    return r;
}

// Overflow detection helpers
bool EvalVisitor::willOverflowAdd(int a, int b) {
    // Check if a + b would overflow
    if (b > 0 && a > INT_MAX - b) return true;
    if (b < 0 && a < INT_MIN - b) return true;
    return false;
}

bool EvalVisitor::willOverflowSubtract(int a, int b) {
    // Check if a - b would overflow
    // a - b is equivalent to a + (-b)
    if (b < 0 && a > INT_MAX + b) return true;
    if (b > 0 && a < INT_MIN + b) return true;
    return false;
}

bool EvalVisitor::willOverflowMultiply(int a, int b) {
    // Check if a * b would overflow
    if (a == 0 || b == 0) return false;
    
    // Check for INT_MIN * -1 special case
    if (a == -1 && b == INT_MIN) return true;
    if (b == -1 && a == INT_MIN) return true;
    
    // Use division to check: if a * b would overflow, then a > INT_MAX / b (for positive)
    if (a > 0 && b > 0) {
        if (a > INT_MAX / b) return true;
    } else if (a < 0 && b < 0) {
        if (a < INT_MAX / b) return true;
    } else if (a > 0 && b < 0) {
        if (b < INT_MIN / a) return true;
    } else if (a < 0 && b > 0) {
        if (a < INT_MIN / b) return true;
    }
    
    return false;
}

bool EvalVisitor::willOverflowFloorDiv(int a, int b) {
    // Check if floor division would overflow
    // The only case is INT_MIN / -1 which would give INT_MAX + 1
    if (b == 0) return false; // Will throw division by zero, not overflow
    if (a == INT_MIN && b == -1) return true;
    return false;
}

bool EvalVisitor::willOverflowModulo(int a, int b) {
    // Modulo operation doesn't overflow in Python semantics
    // The only problematic case in C++ is INT_MIN % -1, but Python handles this
    return false;
}

void EvalVisitor::findAssignedVariables(Python3Parser::SuiteContext* suite, std::set<std::string>& assigned) {
    // A suite contains either:
    // 1. simple_stmt (singular, on the same line as the colon)
    // 2. NEWLINE INDENT stmt+ DEDENT (multi-line block)
    
    auto simple_stmt = suite->simple_stmt();
    if (simple_stmt) {
        auto small_stmt = simple_stmt->small_stmt();
        if (small_stmt) {
            // Check if this is an expression statement
            auto expr_stmt = small_stmt->expr_stmt();
            if (expr_stmt) {
                // expr_stmt: testlist ( (augassign testlist) | ('=' testlist)* )
                auto testlists = expr_stmt->testlist();
                if (!testlists.empty()) {
                    // The first testlist contains the assigned variable(s)
                    std::string varText = testlists[0]->getText();
                    if (varText.find(',') != std::string::npos) {
                        // Tuple unpacking - split by comma
                        size_t start = 0;
                        size_t comma = varText.find(',');
                        while (comma != std::string::npos) {
                            std::string var = varText.substr(start, comma - start);
                            var.erase(0, var.find_first_not_of(" \t"));
                            var.erase(var.find_last_not_of(" \t") + 1);
                            if (!var.empty()) assigned.insert(var);
                            start = comma + 1;
                            comma = varText.find(',', start);
                        }
                        std::string var = varText.substr(start);
                        var.erase(0, var.find_first_not_of(" \t"));
                        var.erase(var.find_last_not_of(" \t") + 1);
                        if (!var.empty()) assigned.insert(var);
                    } else {
                        // Simple assignment
                        assigned.insert(varText);
                    }
                }
            }
        }
    }
    
    auto stmts = suite->stmt();
    for (auto stmt : stmts) {
        findAssignedInStmt(stmt, assigned);
    }
}

void EvalVisitor::findAssignedInStmt(Python3Parser::StmtContext* stmt, std::set<std::string>& assigned) {
    // Check simple statements
    auto simple_stmt = stmt->simple_stmt();
    if (simple_stmt) {
        auto small_stmt = simple_stmt->small_stmt();
        if (small_stmt) {
            auto expr_stmt = small_stmt->expr_stmt();
            if (expr_stmt) {
                auto testlists = expr_stmt->testlist();
                if (!testlists.empty()) {
                    std::string varText = testlists[0]->getText();
                    if (varText.find(',') != std::string::npos) {
                        // Tuple unpacking
                        size_t start = 0;
                        size_t comma = varText.find(',');
                        while (comma != std::string::npos) {
                            std::string var = varText.substr(start, comma - start);
                            var.erase(0, var.find_first_not_of(" \t"));
                            var.erase(var.find_last_not_of(" \t") + 1);
                            if (!var.empty()) assigned.insert(var);
                            start = comma + 1;
                            comma = varText.find(',', start);
                        }
                        std::string var = varText.substr(start);
                        var.erase(0, var.find_first_not_of(" \t"));
                        var.erase(var.find_last_not_of(" \t") + 1);
                        if (!var.empty()) assigned.insert(var);
                    } else {
                        assigned.insert(varText);
                    }
                }
            }
        }
    }
    
    // Check compound statements (if, while, etc.)
    auto compound_stmt = stmt->compound_stmt();
    if (compound_stmt) {
        // Check if-statement
        auto if_stmt = compound_stmt->if_stmt();
        if (if_stmt) {
            // Recursively check the if body
            auto suites = if_stmt->suite();
            for (auto suite : suites) {
                findAssignedVariables(suite, assigned);
            }
        }
        
        // Check while-statement
        auto while_stmt = compound_stmt->while_stmt();
        if (while_stmt) {
            auto suite = while_stmt->suite();
            if (suite) {
                findAssignedVariables(suite, assigned);
            }
        }
        
        // We don't need to check funcdef since nested functions create their own scope
    }
}

void EvalVisitor::findGlobalDeclarations(Python3Parser::SuiteContext* suite, std::set<std::string>& globals) {
    // A suite contains either:
    // 1. simple_stmt (singular, on the same line as the colon)
    // 2. NEWLINE INDENT stmt+ DEDENT (multi-line block)
    
    auto simple_stmt = suite->simple_stmt();
    if (simple_stmt) {
        auto small_stmt = simple_stmt->small_stmt();
        if (small_stmt) {
            // Check if this is a global statement
            auto global_stmt = small_stmt->global_stmt();
            if (global_stmt) {
                // Extract all variable names from global statement
                auto names = global_stmt->NAME();
                for (auto name : names) {
                    globals.insert(name->getText());
                }
            }
        }
    }
    
    auto stmts = suite->stmt();
    for (auto stmt : stmts) {
        findGlobalInStmt(stmt, globals);
    }
}

void EvalVisitor::findGlobalInStmt(Python3Parser::StmtContext* stmt, std::set<std::string>& globals) {
    // Check simple statements
    auto simple_stmt = stmt->simple_stmt();
    if (simple_stmt) {
        auto small_stmt = simple_stmt->small_stmt();
        if (small_stmt) {
            auto global_stmt = small_stmt->global_stmt();
            if (global_stmt) {
                // Extract all variable names from global statement
                auto names = global_stmt->NAME();
                for (auto name : names) {
                    globals.insert(name->getText());
                }
            }
        }
    }
    
    // Check compound statements (if, while, etc.)
    auto compound_stmt = stmt->compound_stmt();
    if (compound_stmt) {
        // Check if-statement
        auto if_stmt = compound_stmt->if_stmt();
        if (if_stmt) {
            // Recursively check the if body
            auto suites = if_stmt->suite();
            for (auto suite : suites) {
                findGlobalDeclarations(suite, globals);
            }
        }
        
        // Check while-statement
        auto while_stmt = compound_stmt->while_stmt();
        if (while_stmt) {
            auto suite = while_stmt->suite();
            if (suite) {
                findGlobalDeclarations(suite, globals);
            }
        }
        
        // We don't need to check funcdef since nested functions create their own scope
    }
}

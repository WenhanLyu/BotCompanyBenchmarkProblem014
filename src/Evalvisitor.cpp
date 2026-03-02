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
    
    if (testlists.size() == 1) {
        // Just an expression, evaluate it
        return visit(testlists[0]);
    } else {
        // Assignment: a = b = c = value
        // The last testlist is the value to assign
        // All previous testlists are variables to assign to
        
        // Evaluate the rightmost expression (the value)
        auto valueAny = visit(testlists.back());
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
        
        // Assign to all variables on the left (right to left, excluding the last one)
        for (size_t i = 0; i < testlists.size() - 1; i++) {
            // For simple assignment, testlist should contain a single test with a NAME
            auto testlist = testlists[i];
            auto tests = testlist->test();
            if (!tests.empty()) {
                // Get the variable name from the test
                // We need to traverse: test -> or_test -> and_test -> not_test -> comparison -> arith_expr -> term -> factor -> atom_expr -> atom -> NAME
                std::string varName = tests[0]->getText();
                
                // Store the value in the variables map
                variables[varName] = value;
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
                    // Get the first argument's test expression
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        // Visit the test to get its value
                        auto argValue = visit(tests[0]);
                        
                        // If it's a Value, print it
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                // Print based on the type
                                if (std::holds_alternative<std::string>(val)) {
                                    std::cout << std::get<std::string>(val) << std::endl;
                                } else if (std::holds_alternative<int>(val)) {
                                    std::cout << std::get<int>(val) << std::endl;
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
                                    std::cout << result << std::endl;
                                } else if (std::holds_alternative<bool>(val)) {
                                    // Python prints True/False, not 1/0
                                    std::cout << (std::get<bool>(val) ? "True" : "False") << std::endl;
                                } else if (std::holds_alternative<std::monostate>(val)) {
                                    // Python prints None
                                    std::cout << "None" << std::endl;
                                }
                            } catch (...) {
                                // Not a Value, ignore
                            }
                        }
                    }
                }
            }
        }
        return std::any();
    } else {
        // No trailer - this is just an atom expression, visit the atom
        return visit(atom);
    }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
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
            // Parse as integer
            int numValue = std::stoi(numStr);
            return Value(numValue);
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
        // Look up the variable in the map
        auto it = variables.find(varName);
        if (it != variables.end()) {
            return it->second;
        } else {
            // Variable not found, return None
            return Value(std::monostate{});
        }
    }
    
    // For other atoms, return empty
    return std::any();
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    // Trailer is handled in visitAtom_expr
    return visitChildren(ctx);
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
        if (std::holds_alternative<int>(result) && std::holds_alternative<int>(term)) {
            // int op int -> int
            int left = std::get<int>(result);
            int right = std::get<int>(term);
            if (op == "+") {
                result = left + right;
            } else if (op == "-") {
                result = left - right;
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
        if (op == "/") {
            // Division always returns double
            double left = std::holds_alternative<double>(result) ? std::get<double>(result) : static_cast<double>(std::get<int>(result));
            double right = std::holds_alternative<double>(factor) ? std::get<double>(factor) : static_cast<double>(std::get<int>(factor));
            result = left / right;
        } else if (std::holds_alternative<int>(result) && std::holds_alternative<int>(factor)) {
            // int op int -> int (except /)
            int left = std::get<int>(result);
            int right = std::get<int>(factor);
            if (op == "*") {
                result = left * right;
            } else if (op == "//") {
                result = left / right;
            } else if (op == "%") {
                result = left % right;
            }
        } else {
            // int op double -> double OR double op int -> double OR double op double -> double
            double left = std::holds_alternative<double>(result) ? std::get<double>(result) : static_cast<double>(std::get<int>(result));
            double right = std::holds_alternative<double>(factor) ? std::get<double>(factor) : static_cast<double>(std::get<int>(factor));
            if (op == "*") {
                result = left * right;
            } else if (op == "//") {
                result = static_cast<int>(left) / static_cast<int>(right);
            } else if (op == "%") {
                result = static_cast<int>(left) % static_cast<int>(right);
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
                    return Value(-std::get<int>(factorVal));
                } else if (std::holds_alternative<double>(factorVal)) {
                    return Value(-std::get<double>(factorVal));
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

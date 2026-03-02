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
        // For now, treat all numbers as integers
        int numValue = std::stoi(numStr);
        return Value(numValue);
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

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
    // Visit expression statement
    return visitChildren(ctx);
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
                                if (std::holds_alternative<std::string>(val)) {
                                    std::cout << std::get<std::string>(val) << std::endl;
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
    
    // For other atoms (NAME, NUMBER, etc.), return empty for now
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

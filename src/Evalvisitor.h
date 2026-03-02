#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include <variant>
#include <string>
#include <iostream>
#include <vector>
#include <map>

// Value class using std::variant to represent Python values
// std::monostate represents None in Python
using Value = std::variant<std::monostate, int, bool, std::string, double>;

class EvalVisitor : public Python3ParserBaseVisitor {
public:
    // Entry point for the interpreter
    std::any visitFile_input(Python3Parser::File_inputContext *ctx) override;
    
    // Statement processing
    std::any visitStmt(Python3Parser::StmtContext *ctx) override;
    std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override;
    std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;
    
    // Expression processing
    std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;
    std::any visitAtom(Python3Parser::AtomContext *ctx) override;
    std::any visitTrailer(Python3Parser::TrailerContext *ctx) override;
    
    // Arithmetic expression processing
    std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;
    std::any visitTerm(Python3Parser::TermContext *ctx) override;
    std::any visitFactor(Python3Parser::FactorContext *ctx) override;

private:
    // Variable storage: maps variable names to their values
    std::map<std::string, Value> variables;
    
    // Helper to remove quotes from string literals
    std::string unquoteString(const std::string& str);
};

#endif//PYTHON_INTERPRETER_EVALVISITOR_H

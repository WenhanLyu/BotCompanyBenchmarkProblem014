#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include <variant>
#include <string>
#include <iostream>

// Value class using std::variant to represent Python values
using Value = std::variant<std::monostate, std::string>;

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
};

#endif//PYTHON_INTERPRETER_EVALVISITOR_H

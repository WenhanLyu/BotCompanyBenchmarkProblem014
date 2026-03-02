#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include <variant>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>

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
    
    // Comparison expression processing
    std::any visitComparison(Python3Parser::ComparisonContext *ctx) override;
    
    // Control flow statements
    std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override;
    std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override;
    std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override;
    std::any visitSuite(Python3Parser::SuiteContext *ctx) override;
    
    // Test expressions (for conditions)
    std::any visitTest(Python3Parser::TestContext *ctx) override;
    std::any visitOr_test(Python3Parser::Or_testContext *ctx) override;
    std::any visitAnd_test(Python3Parser::And_testContext *ctx) override;
    std::any visitNot_test(Python3Parser::Not_testContext *ctx) override;

private:
    // Variable storage: maps variable names to their values
    std::map<std::string, Value> variables;
    
    // Helper to remove quotes from string literals
    std::string unquoteString(const std::string& str);
    
    // Helper to convert Value to bool for condition evaluation
    bool valueToBool(const Value& val);
};

#endif//PYTHON_INTERPRETER_EVALVISITOR_H

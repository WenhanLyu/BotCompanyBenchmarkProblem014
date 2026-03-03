#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "BigInteger.h"
#include <variant>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <climits>

// Value class using std::variant to represent Python values
// std::monostate represents None in Python
using Value = std::variant<std::monostate, int, bool, std::string, double, BigInteger>;

// Exception class for handling return statements in functions
class ReturnException : public std::exception {
public:
    Value returnValue;
    
    explicit ReturnException(const Value& val) : returnValue(val) {}
    
    const char* what() const noexcept override {
        return "Return statement executed";
    }
};

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
    std::any visitFormat_string(Python3Parser::Format_stringContext *ctx) override;
    
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
    
    // Function definition
    std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;
    
    // Return statement
    std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override;

private:
    // Structure to store function definitions
    struct FunctionDef {
        std::vector<std::string> parameters;  // Parameter names
        Python3Parser::SuiteContext* body;     // Function body (suite)
        std::set<std::string> assignedVars;    // Variables assigned in function (local vars)
    };
    
    // Variable storage: maps variable names to their values
    std::map<std::string, Value> variables;
    
    // Function storage: maps function names to their definitions
    std::map<std::string, FunctionDef> functions;
    
    // Local variable storage during function execution (nullptr when not in function)
    std::map<std::string, Value>* localVariables = nullptr;
    
    // Set of variables that are local in the current function (nullptr when not in function)
    const std::set<std::string>* currentFunctionLocals = nullptr;
    
    // Helper to remove quotes from string literals
    std::string unquoteString(const std::string& str);
    
    // Helper to convert Value to string for printing and f-strings
    std::string valueToString(const Value& val);
    
    // Helper to convert Value to bool for condition evaluation
    bool valueToBool(const Value& val);
    
    // Python-style floor division for integers (floors toward -∞)
    int pythonFloorDiv(int a, int b);
    
    // Python-style modulo for integers (result has same sign as divisor)
    int pythonModulo(int a, int b);
    
    // Overflow detection helpers - check BEFORE operation
    bool willOverflowAdd(int a, int b);
    bool willOverflowSubtract(int a, int b);
    bool willOverflowMultiply(int a, int b);
    bool willOverflowFloorDiv(int a, int b);
    bool willOverflowModulo(int a, int b);
    
    // Helper to find all variables assigned in a function body
    void findAssignedVariables(Python3Parser::SuiteContext* suite, std::set<std::string>& assigned);
    void findAssignedInStmt(Python3Parser::StmtContext* stmt, std::set<std::string>& assigned);
};

#endif//PYTHON_INTERPRETER_EVALVISITOR_H

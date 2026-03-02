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
    // Visit atom expression (atom followed by optional trailers)
    return visitChildren(ctx);
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Visit atom (string, number, etc.)
    return visitChildren(ctx);
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    // Visit trailer (function call, subscript, etc.)
    return visitChildren(ctx);
}

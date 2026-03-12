#include "Evalvisitor.h"

bool TupleValue::operator==(const TupleValue& other) const {
    return elements == other.elements;
}
bool ListValue::operator==(const ListValue& other) const {
    return *elements == *other.elements;
}

// Forward declare for lexicographic comparison
static int compareValues(const Value& a, const Value& b);

// Compare two values: returns -1 if a < b, 0 if a == b, 1 if a > b
// Supports: int, bool, double, BigInteger, string, list, tuple
static int compareValues(const Value& a, const Value& b) {
    // Helper: convert numeric to double
    auto toDouble = [](const Value& v) -> double {
        if (std::holds_alternative<int>(v)) return std::get<int>(v);
        if (std::holds_alternative<bool>(v)) return std::get<bool>(v) ? 1.0 : 0.0;
        if (std::holds_alternative<double>(v)) return std::get<double>(v);
        return 0.0;
    };
    bool aIsNum = std::holds_alternative<int>(a) || std::holds_alternative<bool>(a) || std::holds_alternative<double>(a);
    bool bIsNum = std::holds_alternative<int>(b) || std::holds_alternative<bool>(b) || std::holds_alternative<double>(b);
    bool aIsBig = std::holds_alternative<BigInteger>(a);
    bool bIsBig = std::holds_alternative<BigInteger>(b);
    
    if ((aIsNum || aIsBig) && (bIsNum || bIsBig)) {
        if (aIsBig && bIsBig) {
            const BigInteger& la = std::get<BigInteger>(a);
            const BigInteger& rb = std::get<BigInteger>(b);
            if (la < rb) return -1;
            if (rb < la) return 1;
            return 0;
        } else if (aIsBig) {
            BigInteger rb((long long)toDouble(b));
            if (std::get<BigInteger>(a) < rb) return -1;
            if (rb < std::get<BigInteger>(a)) return 1;
            return 0;
        } else if (bIsBig) {
            BigInteger la((long long)toDouble(a));
            if (la < std::get<BigInteger>(b)) return -1;
            if (std::get<BigInteger>(b) < la) return 1;
            return 0;
        } else {
            double da = toDouble(a), db = toDouble(b);
            if (da < db) return -1;
            if (da > db) return 1;
            return 0;
        }
    } else if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) {
        const std::string& sa = std::get<std::string>(a);
        const std::string& sb = std::get<std::string>(b);
        if (sa < sb) return -1;
        if (sa > sb) return 1;
        return 0;
    } else if (std::holds_alternative<ListValue>(a) && std::holds_alternative<ListValue>(b)) {
        // Lexicographic comparison of lists
        const auto& la = *std::get<ListValue>(a).elements;
        const auto& lb = *std::get<ListValue>(b).elements;
        size_t minLen = std::min(la.size(), lb.size());
        for (size_t i = 0; i < minLen; i++) {
            int c = compareValues(la[i], lb[i]);
            if (c != 0) return c;
        }
        if (la.size() < lb.size()) return -1;
        if (la.size() > lb.size()) return 1;
        return 0;
    } else if (std::holds_alternative<TupleValue>(a) && std::holds_alternative<TupleValue>(b)) {
        // Lexicographic comparison of tuples
        const auto& ta = std::get<TupleValue>(a).elements;
        const auto& tb = std::get<TupleValue>(b).elements;
        size_t minLen = std::min(ta.size(), tb.size());
        for (size_t i = 0; i < minLen; i++) {
            int c = compareValues(ta[i], tb[i]);
            if (c != 0) return c;
        }
        if (ta.size() < tb.size()) return -1;
        if (ta.size() > tb.size()) return 1;
        return 0;
    }
    return 0; // incomparable types treated as equal
}

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
        // Check if LHS is a subscript expression for augmented assignment
        // e.g., lst[0] += 5 or matrix[i][j] += 1
        if (testlists.size() >= 2) {
            auto lhsTests = testlists[0]->test();
            if (!lhsTests.empty()) {
                auto lhsAtomExpr = getAtomExprFromTest(lhsTests[0]);
                if (lhsAtomExpr && lhsAtomExpr->trailer().size() >= 2 &&
                    lhsAtomExpr->trailer(0)->OPEN_BRACK() &&
                    lhsAtomExpr->trailer(1)->OPEN_BRACK()) {
                    // 2-trailer case: m[i][j] op= val
                    std::string outerVarName = lhsAtomExpr->atom()->getText();
                    Value* outerPtr = nullptr;
                    if (localVariables != nullptr) {
                        auto it = localVariables->find(outerVarName);
                        if (it != localVariables->end()) outerPtr = &it->second;
                    }
                    if (!outerPtr && enclosingLocalVariables != nullptr) {
                        // Check enclosing scope (for nested functions accessing outer locals)
                        auto eit = enclosingLocalVariables->find(outerVarName);
                        if (eit != enclosingLocalVariables->end()) outerPtr = &eit->second;
                    }
                    if (!outerPtr) {
                        auto it = variables.find(outerVarName);
                        if (it != variables.end()) outerPtr = &it->second;
                    }
                    if (outerPtr && std::holds_alternative<ListValue>(*outerPtr)) {
                        ListValue& outerList = std::get<ListValue>(*outerPtr);
                        // Evaluate outer index
                        auto idx1Any = visit(lhsAtomExpr->trailer(0)->test());
                        int idx1 = 0;
                        if (idx1Any.has_value()) {
                            try {
                                Value idxVal = std::any_cast<Value>(idx1Any);
                                if (std::holds_alternative<int>(idxVal)) idx1 = std::get<int>(idxVal);
                                else if (std::holds_alternative<bool>(idxVal)) idx1 = std::get<bool>(idxVal) ? 1 : 0;
                                else if (std::holds_alternative<BigInteger>(idxVal)) idx1 = static_cast<int>(std::get<BigInteger>(idxVal).toLongLong());
                            } catch (...) {}
                        }
                        if (idx1 < 0) idx1 += static_cast<int>(outerList.elements->size());
                        if (idx1 >= 0 && idx1 < static_cast<int>(outerList.elements->size()) &&
                            std::holds_alternative<ListValue>((*outerList.elements)[idx1])) {
                            ListValue& innerList = std::get<ListValue>((*outerList.elements)[idx1]);
                            // Evaluate inner index
                            auto idx2Any = visit(lhsAtomExpr->trailer(1)->test());
                            int idx2 = 0;
                            if (idx2Any.has_value()) {
                                try {
                                    Value idxVal = std::any_cast<Value>(idx2Any);
                                    if (std::holds_alternative<int>(idxVal)) idx2 = std::get<int>(idxVal);
                                    else if (std::holds_alternative<bool>(idxVal)) idx2 = std::get<bool>(idxVal) ? 1 : 0;
                                    else if (std::holds_alternative<BigInteger>(idxVal)) idx2 = static_cast<int>(std::get<BigInteger>(idxVal).toLongLong());
                                } catch (...) {}
                            }
                            if (idx2 < 0) idx2 += static_cast<int>(innerList.elements->size());
                            if (idx2 >= 0 && idx2 < static_cast<int>(innerList.elements->size())) {
                                Value currentVal = (*innerList.elements)[idx2];
                                auto rightAny = visit(testlists[1]);
                                Value rightVal;
                                if (rightAny.has_value()) {
                                    try { rightVal = std::any_cast<Value>(rightAny); } catch (...) { rightVal = Value(0); }
                                } else { rightVal = Value(0); }
                                std::string op = augassign->getText();
                                if (std::holds_alternative<bool>(currentVal)) currentVal = Value(std::get<bool>(currentVal) ? 1 : 0);
                                if (std::holds_alternative<bool>(rightVal)) rightVal = Value(std::get<bool>(rightVal) ? 1 : 0);
                                Value newVal;
                                if (op == "+=") {
                                    if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                        int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                        if (willOverflowAdd(l, r)) newVal = tryDowncastBigInteger(BigInteger(l) + BigInteger(r));
                                        else newVal = l + r;
                                    } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                        BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                        BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                        newVal = tryDowncastBigInteger(l + r);
                                    } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                        double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                        double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                        newVal = l + r;
                                    } else { newVal = currentVal; }
                                } else if (op == "-=") {
                                    if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                        int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                        newVal = willOverflowSubtract(l,r) ? tryDowncastBigInteger(BigInteger(l)-BigInteger(r)) : Value(l-r);
                                    } else { newVal = currentVal; }
                                } else if (op == "*=") {
                                    if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                        int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                        newVal = willOverflowMultiply(l,r) ? tryDowncastBigInteger(BigInteger(l)*BigInteger(r)) : Value(l*r);
                                    } else { newVal = currentVal; }
                                } else if (op == "/=") {
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    if (r != 0.0) newVal = l / r; else newVal = currentVal;
                                } else if (op == "//=") {
                                    if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                        newVal = pythonFloorDiv(std::get<int>(currentVal), std::get<int>(rightVal));
                                    } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                        double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                        double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                        newVal = std::floor(l / r);
                                    } else { newVal = currentVal; }
                                } else if (op == "%=") {
                                    if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                        newVal = pythonModulo(std::get<int>(currentVal), std::get<int>(rightVal));
                                    } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                        double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                        double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                        newVal = l - std::floor(l / r) * r;
                                    } else { newVal = currentVal; }
                                } else if (op == "**=") {
                                    newVal = powerValue(currentVal, rightVal);
                                } else { newVal = currentVal; }
                                (*innerList.elements)[idx2] = newVal;
                            }
                        }
                    }
                    return std::any();
                } else if (lhsAtomExpr && !lhsAtomExpr->trailer().empty() &&
                    lhsAtomExpr->trailer(0)->OPEN_BRACK()) {
                    // This is a subscript augmented assignment: lst[i] op= val
                    std::string listVarName = lhsAtomExpr->atom()->getText();
                    Value* listPtr = nullptr;
                    if (localVariables != nullptr) {
                        auto it = localVariables->find(listVarName);
                        if (it != localVariables->end()) listPtr = &it->second;
                    }
                    if (!listPtr && enclosingLocalVariables != nullptr) {
                        // Check enclosing scope (for nested functions accessing outer locals)
                        auto eit = enclosingLocalVariables->find(listVarName);
                        if (eit != enclosingLocalVariables->end()) listPtr = &eit->second;
                    }
                    if (!listPtr) {
                        auto it = variables.find(listVarName);
                        if (it != variables.end()) listPtr = &it->second;
                    }
                    if (listPtr && std::holds_alternative<ListValue>(*listPtr)) {
                        ListValue& lst = std::get<ListValue>(*listPtr);
                        auto idxAny = visit(lhsAtomExpr->trailer(0)->test());
                        int idx = 0;
                        if (idxAny.has_value()) {
                            try {
                                Value idxVal = std::any_cast<Value>(idxAny);
                                if (std::holds_alternative<int>(idxVal)) idx = std::get<int>(idxVal);
                                else if (std::holds_alternative<bool>(idxVal)) idx = std::get<bool>(idxVal) ? 1 : 0;
                                else if (std::holds_alternative<BigInteger>(idxVal)) {
                                    idx = static_cast<int>(std::get<BigInteger>(idxVal).toLongLong());
                                }
                            } catch (...) {}
                        }
                        if (idx < 0) idx += static_cast<int>(lst.elements->size());
                        if (idx >= 0 && idx < static_cast<int>(lst.elements->size())) {
                            Value currentVal = (*lst.elements)[idx];
                            auto rightAny = visit(testlists[1]);
                            Value rightVal;
                            if (rightAny.has_value()) {
                                try { rightVal = std::any_cast<Value>(rightAny); } catch (...) { rightVal = Value(0); }
                            } else { rightVal = Value(0); }
                            std::string op = augassign->getText();
                            if (std::holds_alternative<bool>(currentVal)) currentVal = Value(std::get<bool>(currentVal) ? 1 : 0);
                            if (std::holds_alternative<bool>(rightVal)) rightVal = Value(std::get<bool>(rightVal) ? 1 : 0);
                            Value newVal;
                            if (op == "+=") {
                                if (std::holds_alternative<ListValue>(currentVal) && std::holds_alternative<ListValue>(rightVal)) {
                                    // List in-place extension via shared_ptr (reference semantics preserved)
                                    ListValue& lhs = std::get<ListValue>(currentVal);
                                    const ListValue& rhs = std::get<ListValue>(rightVal);
                                    for (const auto& elem : *rhs.elements) {
                                        lhs.elements->push_back(elem);
                                    }
                                    newVal = currentVal;  // Same shared_ptr - inner list is modified
                                } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                    BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                    BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                    newVal = tryDowncastBigInteger(l + r);
                                } else if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                    if (willOverflowAdd(l, r)) newVal = tryDowncastBigInteger(BigInteger(l) + BigInteger(r));
                                    else newVal = l + r;
                                } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    newVal = l + r;
                                } else if (std::holds_alternative<std::string>(currentVal) && std::holds_alternative<std::string>(rightVal)) {
                                    newVal = std::get<std::string>(currentVal) + std::get<std::string>(rightVal);
                                } else { newVal = currentVal; }
                            } else if (op == "-=") {
                                if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                    if (willOverflowSubtract(l, r)) newVal = tryDowncastBigInteger(BigInteger(l) - BigInteger(r));
                                    else newVal = l - r;
                                } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                    BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                    BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                    newVal = tryDowncastBigInteger(l - r);
                                } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    newVal = l - r;
                                } else { newVal = currentVal; }
                            } else if (op == "*=") {
                                if (std::holds_alternative<ListValue>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    // List in-place repetition via shared_ptr
                                    ListValue& lst_inner = std::get<ListValue>(currentVal);
                                    int count = std::get<int>(rightVal);
                                    std::vector<Value> repeated;
                                    repeated.reserve(lst_inner.elements->size() * (count > 0 ? count : 0));
                                    for (int ki = 0; ki < count; ki++) {
                                        for (const auto& elem : *lst_inner.elements) {
                                            repeated.push_back(elem);
                                        }
                                    }
                                    lst_inner.elements->assign(repeated.begin(), repeated.end());
                                    newVal = currentVal;
                                } else if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                    if (willOverflowMultiply(l, r)) newVal = tryDowncastBigInteger(BigInteger(l) * BigInteger(r));
                                    else newVal = l * r;
                                } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                    BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                    BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                    newVal = tryDowncastBigInteger(l * r);
                                } else if (std::holds_alternative<double>(currentVal) || std::holds_alternative<double>(rightVal)) {
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    newVal = l * r;
                                } else { newVal = currentVal; }
                            } else if (op == "//=") {
                                if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                    newVal = pythonFloorDiv(l, r);
                                } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                    BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                    BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                    newVal = tryDowncastBigInteger(l.floorDiv(r));
                                } else {
                                    // Float floor division: floor toward -infinity, result is float
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    newVal = std::floor(l / r);
                                }
                            } else if (op == "%=") {
                                if (std::holds_alternative<int>(currentVal) && std::holds_alternative<int>(rightVal)) {
                                    int l = std::get<int>(currentVal), r = std::get<int>(rightVal);
                                    newVal = pythonModulo(l, r);
                                } else if (std::holds_alternative<BigInteger>(currentVal) || std::holds_alternative<BigInteger>(rightVal)) {
                                    BigInteger l = std::holds_alternative<BigInteger>(currentVal) ? std::get<BigInteger>(currentVal) : BigInteger(std::get<int>(currentVal));
                                    BigInteger r = std::holds_alternative<BigInteger>(rightVal) ? std::get<BigInteger>(rightVal) : BigInteger(std::get<int>(rightVal));
                                    newVal = tryDowncastBigInteger(l % r);
                                } else {
                                    // Float modulo: a - floor(a/b)*b, result is float
                                    double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) : static_cast<double>(std::get<int>(currentVal));
                                    double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) : static_cast<double>(std::get<int>(rightVal));
                                    newVal = l - std::floor(l / r) * r;
                                }
                            } else if (op == "/=") {
                                double l = std::holds_alternative<double>(currentVal) ? std::get<double>(currentVal) :
                                           (std::holds_alternative<int>(currentVal) ? static_cast<double>(std::get<int>(currentVal)) :
                                           (std::holds_alternative<BigInteger>(currentVal) ? std::stod(std::get<BigInteger>(currentVal).toString()) : 0.0));
                                double r = std::holds_alternative<double>(rightVal) ? std::get<double>(rightVal) :
                                           (std::holds_alternative<int>(rightVal) ? static_cast<double>(std::get<int>(rightVal)) :
                                           (std::holds_alternative<BigInteger>(rightVal) ? std::stod(std::get<BigInteger>(rightVal).toString()) : 1.0));
                                if (r != 0.0) newVal = l / r;
                                else newVal = currentVal;  // Division by zero protection
                            } else if (op == "**=") {
                                newVal = powerValue(currentVal, rightVal);
                            } else { newVal = currentVal; }
                            (*lst.elements)[idx] = newVal;
                        }
                    }
                    return std::any();
                }
            }
        }

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
                // This is a local variable - look in local scope first
                if (localVariables != nullptr) {
                    auto localIt = localVariables->find(varName);
                    if (localIt != localVariables->end()) {
                        currentValue = localIt->second;
                        found = true;
                    }
                }
                // If not found locally, fall back to global scope
                // (Non-standard Python: global accessible without 'global' keyword)
                if (!found) {
                    auto it = variables.find(varName);
                    if (it != variables.end()) {
                        currentValue = it->second;
                        found = true;
                        isLocal = false;  // write-back to global scope
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
            
            // Promote bool to int for augmented arithmetic (Python: bool IS-A int)
            if (std::holds_alternative<bool>(currentValue)) {
                currentValue = Value(std::get<bool>(currentValue) ? 1 : 0);
            }
            if (std::holds_alternative<bool>(rightValue)) {
                rightValue = Value(std::get<bool>(rightValue) ? 1 : 0);
            }
            
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
                    result = tryDowncastBigInteger(left + right);
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE addition
                    if (willOverflowAdd(left, right)) {
                        result = tryDowncastBigInteger(BigInteger(left) + BigInteger(right));
                    } else {
                        result = left + right;
                    }
                } else if (std::holds_alternative<double>(currentValue) || std::holds_alternative<double>(rightValue)) {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    result = left + right;
                } else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<std::string>(rightValue)) {
                    result = std::get<std::string>(currentValue) + std::get<std::string>(rightValue);
                } else if (std::holds_alternative<ListValue>(currentValue) && std::holds_alternative<ListValue>(rightValue)) {
                    // List concatenation: extend in-place so caller's list is modified
                    ListValue& lhs = std::get<ListValue>(currentValue);
                    const ListValue& rhs = std::get<ListValue>(rightValue);
                    for (const auto& elem : *rhs.elements) {
                        lhs.elements->push_back(elem);
                    }
                    result = lhs;  // Same shared_ptr - caller's list is modified
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
                    result = tryDowncastBigInteger(left - right);
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE subtraction
                    if (willOverflowSubtract(left, right)) {
                        result = tryDowncastBigInteger(BigInteger(left) - BigInteger(right));
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
                    result = tryDowncastBigInteger(left * right);
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE multiplication
                    if (willOverflowMultiply(left, right)) {
                        result = tryDowncastBigInteger(BigInteger(left) * BigInteger(right));
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
                } else if (std::holds_alternative<ListValue>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    // List repetition: modify in-place so caller's list is modified
                    ListValue& lst = std::get<ListValue>(currentValue);
                    int count = std::get<int>(rightValue);
                    // Snapshot original elements before we modify
                    std::vector<Value> original = *lst.elements;
                    lst.elements->clear();
                    lst.elements->reserve(original.size() * (count > 0 ? count : 0));
                    for (int i = 0; i < count; i++) {
                        for (const auto& elem : original) {
                            lst.elements->push_back(elem);
                        }
                    }
                    result = lst;  // Same shared_ptr
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
                    result = tryDowncastBigInteger(left.floorDiv(right));
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE floor division
                    if (willOverflowFloorDiv(left, right)) {
                        result = tryDowncastBigInteger(BigInteger(left).floorDiv(BigInteger(right)));
                    } else {
                        result = pythonFloorDiv(left, right);
                    }
                } else {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    // Float floor division: floor toward -infinity, result is float
                    result = std::floor(left / right);
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
                    result = tryDowncastBigInteger(left % right);
                } else if (std::holds_alternative<int>(currentValue) && std::holds_alternative<int>(rightValue)) {
                    int left = std::get<int>(currentValue);
                    int right = std::get<int>(rightValue);
                    // Check for overflow BEFORE modulo
                    if (willOverflowModulo(left, right)) {
                        result = tryDowncastBigInteger(BigInteger(left) % BigInteger(right));
                    } else {
                        result = pythonModulo(left, right);
                    }
                } else {
                    double left = std::holds_alternative<double>(currentValue) ? std::get<double>(currentValue) : static_cast<double>(std::get<int>(currentValue));
                    double right = std::holds_alternative<double>(rightValue) ? std::get<double>(rightValue) : static_cast<double>(std::get<int>(rightValue));
                    // Float modulo: a - floor(a/b)*b, result is float
                    result = left - std::floor(left / right) * right;
                }
            } else if (op == "**=") {
                result = powerValue(currentValue, rightValue);
            }
            
            // Store the result (in local scope if local variable, otherwise global)
            // isLocal was already defined above (may have been set to false by global fallback)
            if (isLocal && localVariables != nullptr) {
                (*localVariables)[varName] = result;
                // Also update global if this variable originated from global scope
                // so that multiple augmented assignments (e.g., count += 1; count += 1)
                // all persist to global
                if (variables.find(varName) != variables.end()) {
                    variables[varName] = result;
                }
            } else if (!isLocal && localVariables != nullptr) {
                // It's a parameter or global being modified
                auto localIt = localVariables->find(varName);
                if (localIt != localVariables->end()) {
                    // Modify the parameter (local copy)
                    (*localVariables)[varName] = result;
                } else {
                    // Modify global
                    variables[varName] = result;
                    // Also write to local scope if the pre-scan says this is a local variable
                    // so that subsequent reads (e.g., return x) can find the value
                    if (currentFunctionLocals != nullptr &&
                        currentFunctionLocals->find(varName) != currentFunctionLocals->end()) {
                        (*localVariables)[varName] = result;
                    }
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
        
        // Check how many variables are on the LHS (for tuple unpacking decision)
        bool needsUnpacking = false;
        for (size_t i = 0; i < testlists.size() - 1; i++) {
            auto tests = testlists[i]->test();
            if (tests.size() > 1) {
                needsUnpacking = true;
                break;
            }
        }
        
        // Special case: if there's only one test on the RHS, check if it's a tuple
        if (rightTests.size() == 1) {
            auto valueAny = visit(rightTests[0]);
            if (valueAny.has_value()) {
                try {
                    Value value = std::any_cast<Value>(valueAny);
                    // If the value is a TupleValue or ListValue AND we have multiple vars on LHS, unpack it
                    if (needsUnpacking && std::holds_alternative<TupleValue>(value)) {
                        TupleValue tuple = std::get<TupleValue>(value);
                        values = tuple.elements;
                    } else if (needsUnpacking && std::holds_alternative<ListValue>(value)) {
                        ListValue lst = std::get<ListValue>(value);
                        values = *lst.elements;
                    } else {
                        // Single value or no unpacking needed - keep as-is
                        values.push_back(value);
                    }
                } catch (...) {
                    values.push_back(Value(std::monostate{}));
                }
            } else {
                values.push_back(Value(std::monostate{}));
            }
        } else {
            // Multiple tests on RHS - evaluate each one
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
        }
        
        // Assign to all variables on the left (excluding the last testlist which is the RHS)
        for (size_t i = 0; i < testlists.size() - 1; i++) {
            auto testlist = testlists[i];
            auto tests = testlist->test();
            
            // Check if this is tuple unpacking (multiple tests on the left)
            if (tests.size() > 1) {
                // Tuple unpacking: a, b = expr1, expr2  OR  arr[0], arr[2] = arr[2], arr[0]
                // Assign each value to the corresponding variable
                for (size_t j = 0; j < tests.size(); j++) {
                    Value value = (j < values.size()) ? values[j] : Value(std::monostate{});
                    
                    // Check if this target is a subscript expression (e.g., arr[i])
                    auto lhsAtomExpr = getAtomExprFromTest(tests[j]);
                    if (lhsAtomExpr && !lhsAtomExpr->trailer().empty() && 
                        lhsAtomExpr->trailer(0)->OPEN_BRACK()) {
                        // Subscript assignment: arr[i] = value
                        std::string varName = lhsAtomExpr->atom()->getText();
                        
                        bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
                        Value* varPtr = nullptr;
                        if (!isGlobal && localVariables != nullptr) {
                            auto it = localVariables->find(varName);
                            if (it != localVariables->end()) varPtr = &it->second;
                        }
                        if (!varPtr) {
                            auto it = variables.find(varName);
                            if (it != variables.end()) varPtr = &it->second;
                        }
                        
                        if (varPtr) {
                            auto trailers = lhsAtomExpr->trailer();
                            if (trailers.size() >= 1 && trailers[0]->OPEN_BRACK()) {
                                auto indexTest = trailers[0]->test();
                                auto indexAny = visit(indexTest);
                                Value indexVal = std::any_cast<Value>(indexAny);
                                int index;
                                if (std::holds_alternative<int>(indexVal)) index = std::get<int>(indexVal);
                                else if (std::holds_alternative<bool>(indexVal)) index = std::get<bool>(indexVal) ? 1 : 0;
                                else index = static_cast<int>(std::get<BigInteger>(indexVal).toLongLong());
                                
                                if (std::holds_alternative<ListValue>(*varPtr)) {
                                    ListValue& lst = std::get<ListValue>(*varPtr);
                                    int size = static_cast<int>(lst.elements->size());
                                    if (index < 0) index = size + index;
                                    if (index >= 0 && index < size) {
                                        (*lst.elements)[index] = value;
                                    }
                                }
                            }
                        }
                    } else {
                        // Simple variable target
                        std::string varName = tests[j]->getText();
                        
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
            } else if (!tests.empty()) {
                // Check if LHS is a subscript expression (e.g. lst[0] = val or matrix[i][j] = val)
                auto lhsAtomExpr = getAtomExprFromTest(tests[0]);
                if (lhsAtomExpr && !lhsAtomExpr->trailer().empty() && 
                    lhsAtomExpr->trailer(0)->OPEN_BRACK()) {
                    // This is a subscript assignment: lst[i] = val or matrix[i][j] = val
                    Value value;
                    if (values.size() == 1) {
                        value = values[0];
                    } else {
                        value = TupleValue(values);
                    }
                    
                    // Get the variable name from the atom
                    std::string varName = lhsAtomExpr->atom()->getText();
                    
                    // Look up the variable
                    bool isGlobal = (currentFunctionGlobals.find(varName) != currentFunctionGlobals.end());
                    bool isLocal = !isGlobal && (currentFunctionLocals != nullptr && 
                                   currentFunctionLocals->find(varName) != currentFunctionLocals->end());
                    
                    Value* varPtr = nullptr;
                    if (isLocal && localVariables != nullptr) {
                        auto it = localVariables->find(varName);
                        if (it != localVariables->end()) varPtr = &it->second;
                    } else if (!isLocal && localVariables != nullptr) {
                        auto it = localVariables->find(varName);
                        if (it != localVariables->end()) varPtr = &it->second;
                        if (!varPtr && enclosingLocalVariables != nullptr) {
                            // Check enclosing scope (for nested functions accessing outer locals)
                            auto eit = enclosingLocalVariables->find(varName);
                            if (eit != enclosingLocalVariables->end()) varPtr = &eit->second;
                        }
                        if (!varPtr) {
                            auto git = variables.find(varName);
                            if (git != variables.end()) varPtr = &git->second;
                        }
                    } else {
                        auto it = variables.find(varName);
                        if (it != variables.end()) varPtr = &it->second;
                    }
                    
                    if (varPtr) {
                        auto trailers = lhsAtomExpr->trailer();
                        if (trailers.size() == 1) {
                            // Single subscript: lst[i] = val
                            auto indexTest = trailers[0]->test();
                            auto indexAny = visit(indexTest);
                            Value indexVal = std::any_cast<Value>(indexAny);
                            int index;
                            if (std::holds_alternative<int>(indexVal)) index = std::get<int>(indexVal);
                            else if (std::holds_alternative<bool>(indexVal)) index = std::get<bool>(indexVal) ? 1 : 0;
                            else index = static_cast<int>(std::get<BigInteger>(indexVal).toLongLong());
                            
                            if (std::holds_alternative<ListValue>(*varPtr)) {
                                ListValue& lst = std::get<ListValue>(*varPtr);
                                int size = static_cast<int>(lst.elements->size());
                                if (index < 0) index = size + index;
                                if (index >= 0 && index < size) {
                                    (*lst.elements)[index] = value;
                                }
                            }
                        } else if (trailers.size() == 2) {
                            // Double subscript: matrix[i][j] = val
                            auto idx1Test = trailers[0]->test();
                            auto idx1Any = visit(idx1Test);
                            Value idx1Val = std::any_cast<Value>(idx1Any);
                            int idx1;
                            if (std::holds_alternative<int>(idx1Val)) idx1 = std::get<int>(idx1Val);
                            else if (std::holds_alternative<bool>(idx1Val)) idx1 = std::get<bool>(idx1Val) ? 1 : 0;
                            else idx1 = static_cast<int>(std::get<BigInteger>(idx1Val).toLongLong());
                            
                            auto idx2Test = trailers[1]->test();
                            auto idx2Any = visit(idx2Test);
                            Value idx2Val = std::any_cast<Value>(idx2Any);
                            int idx2;
                            if (std::holds_alternative<int>(idx2Val)) idx2 = std::get<int>(idx2Val);
                            else if (std::holds_alternative<bool>(idx2Val)) idx2 = std::get<bool>(idx2Val) ? 1 : 0;
                            else idx2 = static_cast<int>(std::get<BigInteger>(idx2Val).toLongLong());
                            
                            if (std::holds_alternative<ListValue>(*varPtr)) {
                                ListValue& lst = std::get<ListValue>(*varPtr);
                                int size1 = static_cast<int>(lst.elements->size());
                                if (idx1 < 0) idx1 = size1 + idx1;
                                if (idx1 >= 0 && idx1 < size1 && 
                                    std::holds_alternative<ListValue>((*lst.elements)[idx1])) {
                                    ListValue& innerLst = std::get<ListValue>((*lst.elements)[idx1]);
                                    int size2 = static_cast<int>(innerLst.elements->size());
                                    if (idx2 < 0) idx2 = size2 + idx2;
                                    if (idx2 >= 0 && idx2 < size2) {
                                        (*innerLst.elements)[idx2] = value;
                                    }
                                }
                            }
                        }
                    }
                } else {
                // Simple assignment: a = value (or a = b = c = value)
                std::string varName = tests[0]->getText();
                Value value;
                
                if (values.size() == 1) {
                    value = values[0];
                } else {
                    // If RHS has multiple values but LHS is single, create a tuple
                    value = TupleValue(values);
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
        }
        
        return std::any();
    }
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    // atom_expr: atom trailer*
    // Get the atom (function name or value)
    auto atom = ctx->atom();
    if (!atom) {
        return std::any();
    }
    
    // Get all trailers
    auto trailers = ctx->trailer();
    
    // If no trailers, just evaluate the atom
    if (trailers.empty()) {
        return visit(atom);
    }
    
    // Process trailers left-to-right
    // For the first trailer, we might have a function call
    // For subsequent trailers, we have a value from the previous operation
    std::any currentValue;
    bool isFirstTrailer = true;
    
    for (size_t i = 0; i < trailers.size(); i++) {
        auto trailer = trailers[i];
        
        // Check if this is a subscript operation: '[' test ']'
        if (trailer->OPEN_BRACK()) {
            // This is a subscript operation
            // Get the sequence value (either from atom or from previous operation)
            if (isFirstTrailer) {
                currentValue = visit(atom);
                isFirstTrailer = false;
            }
            
            if (!currentValue.has_value()) {
                throw std::runtime_error("Cannot subscript non-value");
            }
            
            Value sequence = std::any_cast<Value>(currentValue);
            
            // Get the index expression
            auto indexTest = trailer->test();
            if (!indexTest) {
                throw std::runtime_error("Subscript requires an index");
            }
            
            auto indexValue = visit(indexTest);
            if (!indexValue.has_value()) {
                throw std::runtime_error("Index expression must evaluate to a value");
            }
            
            Value indexVal = std::any_cast<Value>(indexValue);
            
            // Index must be an integer, BigInteger, or bool
            int index;
            if (std::holds_alternative<int>(indexVal)) {
                index = std::get<int>(indexVal);
            } else if (std::holds_alternative<bool>(indexVal)) {
                index = std::get<bool>(indexVal) ? 1 : 0;
            } else if (std::holds_alternative<BigInteger>(indexVal)) {
                // Convert BigInteger to int for indexing
                BigInteger bi = std::get<BigInteger>(indexVal);
                try {
                    index = static_cast<int>(bi.toLongLong());
                } catch (...) {
                    throw std::runtime_error("Index too large");
                }
            } else {
                throw std::runtime_error("Index must be an integer");
            }
            
            // Handle subscripting for different types
            if (std::holds_alternative<std::string>(sequence)) {
                // String indexing
                std::string str = std::get<std::string>(sequence);
                int size = static_cast<int>(str.size());
                
                // Handle negative indexing
                if (index < 0) {
                    index = size + index;
                }
                
                // Bounds checking
                if (index < 0 || index >= size) {
                    throw std::runtime_error("String index out of range");
                }
                
                // Set current value to the indexed character
                currentValue = Value(std::string(1, str[index]));
            } else if (std::holds_alternative<TupleValue>(sequence)) {
                // Tuple indexing
                const TupleValue& tuple = std::get<TupleValue>(sequence);
                int size = static_cast<int>(tuple.elements.size());
                
                // Handle negative indexing
                if (index < 0) {
                    index = size + index;
                }
                
                // Bounds checking
                if (index < 0 || index >= size) {
                    throw std::runtime_error("Tuple index out of range");
                }
                
                currentValue = tuple.elements[index];
            } else if (std::holds_alternative<ListValue>(sequence)) {
                // List indexing
                const ListValue& list = std::get<ListValue>(sequence);
                int size = static_cast<int>(list.elements->size());
                
                // Handle negative indexing
                if (index < 0) {
                    index = size + index;
                }
                
                // Bounds checking
                if (index < 0 || index >= size) {
                    throw std::runtime_error("List index out of range");
                }
                
                currentValue = (*list.elements)[index];
            } else {
                throw std::runtime_error("Object is not subscriptable");
            }
        } else {
            // This is a function call trailer (OPEN_PAREN)
            // If i > 0, the "function" is the result of a previous trailer (e.g., ops[0](3,4) or make_adder(5)(3))
            if (i != 0) {
                // currentValue must hold a FunctionValue to be callable
                if (!currentValue.has_value()) {
                    throw std::runtime_error("Cannot call non-callable");
                }
                Value calleeVal = std::any_cast<Value>(currentValue);
                if (!std::holds_alternative<FunctionValue>(calleeVal)) {
                    throw std::runtime_error("Object is not callable");
                }
                const FunctionValue& fv = std::get<FunctionValue>(calleeVal);
                std::string calleeName = fv.name;
                const std::map<std::string, Value> capturedLocals = fv.capturedLocals;
                
                auto funcIt2 = functions.find(calleeName);
                if (funcIt2 == functions.end()) {
                    throw std::runtime_error("Function not found: " + calleeName);
                }
                const FunctionDef& funcDef = funcIt2->second;
                
                std::vector<Value> positionalArgs;
                std::map<std::string, Value> keywordArgs;
                bool seenKeyword = false;
                
                auto arglist = trailer->arglist();
                if (arglist) {
                    auto args = arglist->argument();
                    for (auto arg : args) {
                        auto tests = arg->test();
                        if (tests.size() == 2) {
                            seenKeyword = true;
                            std::string paramName = tests[0]->getText();
                            auto argValue = visit(tests[1]);
                            Value val = Value(std::monostate{});
                            if (argValue.has_value()) {
                                try { val = std::any_cast<Value>(argValue); } catch (...) {}
                            }
                            if (keywordArgs.find(paramName) != keywordArgs.end()) {
                                throw std::runtime_error("Duplicate keyword argument: " + paramName);
                            }
                            keywordArgs[paramName] = val;
                        } else if (!tests.empty()) {
                            if (seenKeyword) {
                                throw std::runtime_error("Positional argument follows keyword argument");
                            }
                            auto argValue = visit(tests[0]);
                            Value val = Value(std::monostate{});
                            if (argValue.has_value()) {
                                try { val = std::any_cast<Value>(argValue); } catch (...) {}
                            }
                            positionalArgs.push_back(val);
                        }
                    }
                }
                
                std::map<std::string, Value> localVars;
                std::map<std::string, Value>* savedLocalVariables = localVariables;
                const std::set<std::string>* savedFunctionLocals = currentFunctionLocals;
                std::set<std::string> savedFunctionGlobals = currentFunctionGlobals;
                std::map<std::string, Value>* savedEnclosingLocalVariables = enclosingLocalVariables;
                
                // Inject captured locals for closure support
                for (const auto& [k, v] : capturedLocals) {
                    localVars[k] = v;
                }
                // Inject the function itself for recursive closure calls (allows self-recursion at any depth)
                if (!capturedLocals.empty()) {
                    localVars[calleeName] = Value(FunctionValue(calleeName, capturedLocals));
                }
                
                enclosingLocalVariables = savedLocalVariables;
                localVariables = &localVars;
                currentFunctionLocals = &funcDef.assignedVars;
                currentFunctionGlobals = funcDef.globalVars;
                
                std::set<std::string> boundParams;
                
                if (positionalArgs.size() > funcDef.parameters.size()) {
                    localVariables = savedLocalVariables;
                    currentFunctionLocals = savedFunctionLocals;
                    currentFunctionGlobals = savedFunctionGlobals;
                    enclosingLocalVariables = savedEnclosingLocalVariables;
                    throw std::runtime_error("Too many positional arguments");
                }
                
                for (size_t pi = 0; pi < positionalArgs.size(); pi++) {
                    localVars[funcDef.parameters[pi]] = positionalArgs[pi];
                    boundParams.insert(funcDef.parameters[pi]);
                }
                
                for (const auto& kwarg : keywordArgs) {
                    const std::string& paramName = kwarg.first;
                    auto it = std::find(funcDef.parameters.begin(), funcDef.parameters.end(), paramName);
                    if (it == funcDef.parameters.end()) {
                        localVariables = savedLocalVariables;
                        currentFunctionLocals = savedFunctionLocals;
                        currentFunctionGlobals = savedFunctionGlobals;
                        enclosingLocalVariables = savedEnclosingLocalVariables;
                        throw std::runtime_error("Unknown parameter name: " + paramName);
                    }
                    if (boundParams.find(paramName) != boundParams.end()) {
                        localVariables = savedLocalVariables;
                        currentFunctionLocals = savedFunctionLocals;
                        currentFunctionGlobals = savedFunctionGlobals;
                        enclosingLocalVariables = savedEnclosingLocalVariables;
                        throw std::runtime_error("Parameter " + paramName + " specified multiple times");
                    }
                    localVars[paramName] = kwarg.second;
                    boundParams.insert(paramName);
                }
                
                // Use captured defaults from FunctionValue if available, otherwise funcDef defaults
                const std::vector<Value>& effectiveDefaults = fv.hasOwnDefaults ? fv.capturedDefaults : funcDef.defaultValues;
                
                for (size_t pi = 0; pi < funcDef.parameters.size(); pi++) {
                    const std::string& paramName = funcDef.parameters[pi];
                    if (boundParams.find(paramName) == boundParams.end()) {
                        // Check if this parameter has a default value
                        // Parameters with defaults are the last numDefaultParams params
                        if (pi >= funcDef.parameters.size() - funcDef.numDefaultParams) {
                            localVars[paramName] = effectiveDefaults[pi];
                        } else {
                            localVariables = savedLocalVariables;
                            currentFunctionLocals = savedFunctionLocals;
                            currentFunctionGlobals = savedFunctionGlobals;
                            enclosingLocalVariables = savedEnclosingLocalVariables;
                            throw std::runtime_error("Missing required parameter: " + paramName);
                        }
                    }
                }
                
                Value returnValue = Value(std::monostate{});
                try {
                    visit(funcDef.body);
                } catch (const ReturnException& e) {
                    returnValue = e.returnValue;
                }
                
                localVariables = savedLocalVariables;
                currentFunctionLocals = savedFunctionLocals;
                currentFunctionGlobals = savedFunctionGlobals;
                enclosingLocalVariables = savedEnclosingLocalVariables;
                
                currentValue = returnValue;
                isFirstTrailer = false;
                continue;
            }
            
            // Get the function name from the atom (i == 0 case)
            std::string funcName = atom->getText();
        
        // Handle print function
        if (funcName == "print") {
            // Get the arglist from the trailer
            auto arglist = trailer->arglist();
            // Defaults for sep and end
            std::string sepStr = " ";
            std::string endStr = "\n";
            std::vector<Value> positionalPrintArgs;
            if (arglist) {
                // Get the arguments - separate positional args from keyword args
                auto args = arglist->argument();
                for (auto arg : args) {
                    auto tests = arg->test();
                    if (tests.size() == 2) {
                        // Keyword argument: name=value
                        std::string kwName = tests[0]->getText();
                        auto kwValue = visit(tests[1]);
                        Value kwVal(std::monostate{});
                        if (kwValue.has_value()) {
                            try { kwVal = std::any_cast<Value>(kwValue); } catch (...) {}
                        }
                        if (kwName == "sep") {
                            if (std::holds_alternative<std::string>(kwVal)) {
                                sepStr = std::get<std::string>(kwVal);
                            } else if (std::holds_alternative<std::monostate>(kwVal)) {
                                sepStr = " ";  // sep=None means default space
                            }
                        } else if (kwName == "end") {
                            if (std::holds_alternative<std::string>(kwVal)) {
                                endStr = std::get<std::string>(kwVal);
                            } else if (std::holds_alternative<std::monostate>(kwVal)) {
                                endStr = "\n";  // end=None means default newline
                            }
                        }
                        // Other keyword args (file, flush) are ignored
                    } else if (!tests.empty()) {
                        // Positional argument
                        auto argValue = visit(tests[0]);
                        Value val(std::monostate{});
                        if (argValue.has_value()) {
                            try { val = std::any_cast<Value>(argValue); } catch (...) {}
                        }
                        positionalPrintArgs.push_back(val);
                    }
                }
            }
            // Print positional args joined by sep
            for (size_t i = 0; i < positionalPrintArgs.size(); ++i) {
                if (i > 0) std::cout << sepStr;
                std::cout << valueToString(positionalPrintArgs[i]);
            }
            // Print end (default newline)
            std::cout << endStr;
            currentValue = std::any();
            isFirstTrailer = false;
            continue;  // Allow subsequent trailers (subscripts on return value)
        }
        
        // Handle int() type conversion function
        if (funcName == "int") {
            // Get the argument
            Value intResult(0);
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
                                    intResult = val;
                                } else if (std::holds_alternative<double>(val)) {
                                    // float → int: truncate decimal part
                                    intResult = Value(static_cast<int>(std::get<double>(val)));
                                } else if (std::holds_alternative<bool>(val)) {
                                    // bool → int: True → 1, False → 0
                                    intResult = Value(std::get<bool>(val) ? 1 : 0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    // str → int: parse string as integer
                                    std::string str = std::get<std::string>(val);
                                    // Check if it's a large number that needs BigInteger
                                    bool negative = !str.empty() && str[0] == '-';
                                    std::string absStr = negative ? str.substr(1) : str;
                                    if (absStr.length() > 10 || 
                                        (absStr.length() == 10 && absStr > "2147483647")) {
                                        intResult = Value(BigInteger(str));
                                    } else {
                                        intResult = Value(std::stoi(str));
                                    }
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    intResult = val;
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            currentValue = intResult;
            isFirstTrailer = false;
            continue;
        }
        
        // Handle float() type conversion function
        if (funcName == "float") {
            // Get the argument
            Value floatResult(0.0);
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
                                    floatResult = val;
                                } else if (std::holds_alternative<int>(val)) {
                                    floatResult = Value(static_cast<double>(std::get<int>(val)));
                                } else if (std::holds_alternative<bool>(val)) {
                                    floatResult = Value(std::get<bool>(val) ? 1.0 : 0.0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    std::string str = std::get<std::string>(val);
                                    floatResult = Value(std::stod(str));
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    BigInteger bi = std::get<BigInteger>(val);
                                    try {
                                        long long ll = bi.toLongLong();
                                        floatResult = Value(static_cast<double>(ll));
                                    } catch (...) {
                                        floatResult = Value(std::stod(bi.toString()));
                                    }
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            currentValue = floatResult;
            isFirstTrailer = false;
            continue;
        }
        
        // Handle str() type conversion function
        if (funcName == "str") {
            // Get the argument
            Value strResult(std::string(""));
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
                                    strResult = val;
                                } else if (std::holds_alternative<int>(val)) {
                                    strResult = Value(std::to_string(std::get<int>(val)));
                                } else if (std::holds_alternative<double>(val)) {
                                    // Use Python repr style (1.0, 3.14) consistent with f-string behavior
                                    strResult = Value(floatToRepr(std::get<double>(val)));
                                } else if (std::holds_alternative<bool>(val)) {
                                    strResult = Value(std::get<bool>(val) ? std::string("True") : std::string("False"));
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    strResult = Value(std::get<BigInteger>(val).toString());
                                } else if (std::holds_alternative<ListValue>(val)) {
                                    strResult = Value(valueToRepr(val));
                                } else if (std::holds_alternative<TupleValue>(val)) {
                                    strResult = Value(valueToRepr(val));
                                } else if (std::holds_alternative<std::monostate>(val)) {
                                    strResult = Value(std::string("None"));
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            currentValue = strResult;
            isFirstTrailer = false;
            continue;
        }
        
        // Handle bool() type conversion function
        if (funcName == "bool") {
            // Get the argument
            Value boolResult(false);
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
                                    boolResult = val;
                                } else if (std::holds_alternative<int>(val)) {
                                    boolResult = Value(std::get<int>(val) != 0);
                                } else if (std::holds_alternative<double>(val)) {
                                    boolResult = Value(std::get<double>(val) != 0.0);
                                } else if (std::holds_alternative<std::string>(val)) {
                                    boolResult = Value(!std::get<std::string>(val).empty());
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    boolResult = Value(!std::get<BigInteger>(val).isZero());
                                } else if (std::holds_alternative<std::monostate>(val)) {
                                    boolResult = Value(false);
                                } else if (std::holds_alternative<ListValue>(val)) {
                                    boolResult = Value(!std::get<ListValue>(val).elements->empty());
                                } else if (std::holds_alternative<TupleValue>(val)) {
                                    boolResult = Value(!std::get<TupleValue>(val).elements.empty());
                                }
                            } catch (...) {
                                // Error in conversion
                            }
                        }
                    }
                }
            }
            currentValue = boolResult;
            isFirstTrailer = false;
            continue;
        }

        // Handle len() built-in function
        if (funcName == "len") {
            Value lenResult(0);
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty()) {
                    auto arg = args[0];
                    auto tests = arg->test();
                    if (!tests.empty()) {
                        auto argValue = visit(tests[0]);
                        if (argValue.has_value()) {
                            try {
                                Value val = std::any_cast<Value>(argValue);
                                if (std::holds_alternative<std::string>(val)) {
                                    lenResult = Value((int)std::get<std::string>(val).size());
                                } else if (std::holds_alternative<TupleValue>(val)) {
                                    lenResult = Value((int)std::get<TupleValue>(val).elements.size());
                                } else if (std::holds_alternative<ListValue>(val)) {
                                    lenResult = Value((int)std::get<ListValue>(val).elements->size());
                                }
                            } catch (...) {
                                // Error in len()
                            }
                        }
                    }
                }
            }
            currentValue = lenResult;
            isFirstTrailer = false;
            continue;
        }

        // Handle abs() built-in function
        if (funcName == "abs") {
            Value absResult(0);
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
                                if (std::holds_alternative<int>(val)) {
                                    int v = std::get<int>(val);
                                    absResult = Value(v < 0 ? -v : v);
                                } else if (std::holds_alternative<bool>(val)) {
                                    absResult = Value(std::get<bool>(val) ? 1 : 0);
                                } else if (std::holds_alternative<double>(val)) {
                                    absResult = Value(std::fabs(std::get<double>(val)));
                                } else if (std::holds_alternative<BigInteger>(val)) {
                                    BigInteger bi = std::get<BigInteger>(val);
                                    if (bi.isNegative()) bi = -bi;
                                    absResult = Value(bi);
                                }
                            } catch (...) {}
                        }
                    }
                }
            }
            currentValue = absResult;
            isFirstTrailer = false;
            continue;
        }

        // Handle max() built-in function — returns the maximum of 1+ arguments
        if (funcName == "max") {
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty()) {
                    // Separate positional args from keyword args (specifically key=)
                    std::vector<Value> vals;
                    Value keyFunc = Value(std::monostate{});  // None = no key function
                    
                    for (auto arg : args) {
                        auto tests = arg->test();
                        if (tests.size() == 2) {
                            // Keyword argument: check if it's key=
                            std::string kwName = tests[0]->getText();
                            if (kwName == "key") {
                                auto av = visit(tests[1]);
                                if (av.has_value()) {
                                    try { keyFunc = std::any_cast<Value>(av); } catch (...) {}
                                }
                            }
                            // Ignore other keyword args (like default=)
                        } else if (!tests.empty()) {
                            auto av = visit(tests[0]);
                            if (av.has_value()) {
                                try { vals.push_back(std::any_cast<Value>(av)); } catch (...) {}
                            }
                        }
                    }
                    // If single argument is a list or tuple, unpack it
                    if (vals.size() == 1) {
                        if (std::holds_alternative<ListValue>(vals[0])) {
                            std::vector<Value> elems = *std::get<ListValue>(vals[0]).elements;
                            vals = elems;
                        } else if (std::holds_alternative<TupleValue>(vals[0])) {
                            std::vector<Value> elems = std::get<TupleValue>(vals[0]).elements;
                            vals = elems;
                        }
                    }
                    if (!vals.empty()) {
                        // Helper lambda to apply key function if provided
                        auto applyKey = [&](const Value& v) -> Value {
                            if (std::holds_alternative<FunctionValue>(keyFunc)) {
                                const FunctionValue& fv = std::get<FunctionValue>(keyFunc);
                                auto funcIt2 = functions.find(fv.name);
                                if (funcIt2 != functions.end()) {
                                    const FunctionDef& kfuncDef = funcIt2->second;
                                    std::map<std::string, Value> kLocalVars;
                                    // Inject captured locals from closure
                                    for (const auto& cl : fv.capturedLocals) kLocalVars[cl.first] = cl.second;
                                    // Inject self-reference for recursive closures
                                    kLocalVars[fv.name] = keyFunc;
                                    if (!kfuncDef.parameters.empty()) kLocalVars[kfuncDef.parameters[0]] = v;
                                    auto* savedLV = localVariables;
                                    auto* savedELV = enclosingLocalVariables;
                                    auto* savedFL = currentFunctionLocals;
                                    auto savedFG = currentFunctionGlobals;
                                    enclosingLocalVariables = savedLV;
                                    localVariables = &kLocalVars;
                                    currentFunctionLocals = &kfuncDef.assignedVars;
                                    currentFunctionGlobals = kfuncDef.globalVars;
                                    Value kResult = Value(std::monostate{});
                                    try { visit(kfuncDef.body); } catch (const ReturnException& e) { kResult = e.returnValue; }
                                    localVariables = savedLV;
                                    enclosingLocalVariables = savedELV;
                                    currentFunctionLocals = savedFL;
                                    currentFunctionGlobals = savedFG;
                                    return kResult;
                                }
                            }
                            return v;  // No key function, use value itself
                        };
                        
                        Value maxVal = vals[0];
                        Value maxKey = applyKey(vals[0]);
                        for (size_t i = 1; i < vals.size(); i++) {
                            Value k = applyKey(vals[i]);
                            if (compareValues(k, maxKey) > 0) {
                                maxVal = vals[i];
                                maxKey = k;
                            }
                        }
                        currentValue = maxVal;
                        isFirstTrailer = false;
                        continue;
                    }
                }
            }
            currentValue = Value(0);
            isFirstTrailer = false;
            continue;
        }

        // Handle min() built-in function — returns the minimum of 1+ arguments
        if (funcName == "min") {
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty()) {
                    std::vector<Value> vals;
                    Value keyFunc = Value(std::monostate{});  // None = no key function
                    
                    for (auto arg : args) {
                        auto tests = arg->test();
                        if (tests.size() == 2) {
                            std::string kwName = tests[0]->getText();
                            if (kwName == "key") {
                                auto av = visit(tests[1]);
                                if (av.has_value()) {
                                    try { keyFunc = std::any_cast<Value>(av); } catch (...) {}
                                }
                            }
                        } else if (!tests.empty()) {
                            auto av = visit(tests[0]);
                            if (av.has_value()) {
                                try { vals.push_back(std::any_cast<Value>(av)); } catch (...) {}
                            }
                        }
                    }
                    // If single argument is a list or tuple, unpack it
                    if (vals.size() == 1) {
                        if (std::holds_alternative<ListValue>(vals[0])) {
                            std::vector<Value> elems = *std::get<ListValue>(vals[0]).elements;
                            vals = elems;
                        } else if (std::holds_alternative<TupleValue>(vals[0])) {
                            std::vector<Value> elems = std::get<TupleValue>(vals[0]).elements;
                            vals = elems;
                        }
                    }
                    if (!vals.empty()) {
                        auto applyKey = [&](const Value& v) -> Value {
                            if (std::holds_alternative<FunctionValue>(keyFunc)) {
                                const FunctionValue& fv = std::get<FunctionValue>(keyFunc);
                                auto funcIt2 = functions.find(fv.name);
                                if (funcIt2 != functions.end()) {
                                    const FunctionDef& kfuncDef = funcIt2->second;
                                    std::map<std::string, Value> kLocalVars;
                                    for (const auto& cl : fv.capturedLocals) kLocalVars[cl.first] = cl.second;
                                    kLocalVars[fv.name] = keyFunc;
                                    if (!kfuncDef.parameters.empty()) kLocalVars[kfuncDef.parameters[0]] = v;
                                    auto* savedLV = localVariables;
                                    auto* savedELV = enclosingLocalVariables;
                                    auto* savedFL = currentFunctionLocals;
                                    auto savedFG = currentFunctionGlobals;
                                    enclosingLocalVariables = savedLV;
                                    localVariables = &kLocalVars;
                                    currentFunctionLocals = &kfuncDef.assignedVars;
                                    currentFunctionGlobals = kfuncDef.globalVars;
                                    Value kResult = Value(std::monostate{});
                                    try { visit(kfuncDef.body); } catch (const ReturnException& e) { kResult = e.returnValue; }
                                    localVariables = savedLV;
                                    enclosingLocalVariables = savedELV;
                                    currentFunctionLocals = savedFL;
                                    currentFunctionGlobals = savedFG;
                                    return kResult;
                                }
                            }
                            return v;
                        };
                        
                        Value minVal = vals[0];
                        Value minKey = applyKey(vals[0]);
                        for (size_t i = 1; i < vals.size(); i++) {
                            Value k = applyKey(vals[i]);
                            if (compareValues(k, minKey) < 0) {
                                minVal = vals[i];
                                minKey = k;
                            }
                        }
                        currentValue = minVal;
                        isFirstTrailer = false;
                        continue;
                    }
                }
            }
            currentValue = Value(0);
            isFirstTrailer = false;
            continue;
        }
        
        if (funcName == "sorted") {
            // sorted(iterable, key=None) → returns a new sorted list (non-destructive)
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                if (!args.empty()) {
                    // First positional arg is the iterable, key= is optional
                    Value iterableVal;
                    Value keyFunc = Value(std::monostate{});
                    bool hasIterable = false;
                    
                    for (auto arg : args) {
                        auto tests = arg->test();
                        if (tests.size() == 2) {
                            std::string kwName = tests[0]->getText();
                            if (kwName == "key") {
                                auto av = visit(tests[1]);
                                if (av.has_value()) {
                                    try { keyFunc = std::any_cast<Value>(av); } catch (...) {}
                                }
                            }
                        } else if (!tests.empty() && !hasIterable) {
                            auto av = visit(tests[0]);
                            if (av.has_value()) {
                                try { iterableVal = std::any_cast<Value>(av); hasIterable = true; } catch (...) {}
                            }
                        }
                    }
                    
                    if (hasIterable) {
                        try {
                            std::vector<Value> elems;
                            if (std::holds_alternative<ListValue>(iterableVal)) {
                                elems = *std::get<ListValue>(iterableVal).elements;
                            } else if (std::holds_alternative<TupleValue>(iterableVal)) {
                                elems = std::get<TupleValue>(iterableVal).elements;
                            }
                            
                            // Apply key function if provided
                            if (std::holds_alternative<FunctionValue>(keyFunc)) {
                                const FunctionValue& fv = std::get<FunctionValue>(keyFunc);
                                auto funcIt2 = functions.find(fv.name);
                                if (funcIt2 != functions.end()) {
                                    const FunctionDef& kfuncDef = funcIt2->second;
                                    // Compute keys for all elements
                                    std::vector<Value> keys;
                                    for (const auto& elem : elems) {
                                        std::map<std::string, Value> kLocalVars;
                                        for (const auto& cl : fv.capturedLocals) kLocalVars[cl.first] = cl.second;
                                        kLocalVars[fv.name] = keyFunc;
                                        if (!kfuncDef.parameters.empty()) kLocalVars[kfuncDef.parameters[0]] = elem;
                                        auto* savedLV = localVariables;
                                        auto* savedELV = enclosingLocalVariables;
                                        auto* savedFL = currentFunctionLocals;
                                        auto savedFG = currentFunctionGlobals;
                                        enclosingLocalVariables = savedLV;
                                        localVariables = &kLocalVars;
                                        currentFunctionLocals = &kfuncDef.assignedVars;
                                        currentFunctionGlobals = kfuncDef.globalVars;
                                        Value kResult = Value(std::monostate{});
                                        try { visit(kfuncDef.body); } catch (const ReturnException& e) { kResult = e.returnValue; }
                                        localVariables = savedLV;
                                        enclosingLocalVariables = savedELV;
                                        currentFunctionLocals = savedFL;
                                        currentFunctionGlobals = savedFG;
                                        keys.push_back(kResult);
                                    }
                                    // Sort by key (using indices to avoid extra copies)
                                    std::vector<size_t> indices(elems.size());
                                    std::iota(indices.begin(), indices.end(), 0);
                                    std::stable_sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
                                        return compareValues(keys[a], keys[b]) < 0;
                                    });
                                    std::vector<Value> sortedElems;
                                    sortedElems.reserve(elems.size());
                                    for (size_t idx : indices) sortedElems.push_back(elems[idx]);
                                    currentValue = Value(ListValue(sortedElems));
                                    isFirstTrailer = false;
                                    continue;
                                }
                            }
                            
                            // No key function - sort normally
                            std::stable_sort(elems.begin(), elems.end(), [](const Value& a, const Value& b) {
                                return compareValues(a, b) < 0;
                            });
                            currentValue = Value(ListValue(elems));
                            isFirstTrailer = false;
                            continue;
                        } catch (...) {}
                    }
                }
            }
            currentValue = Value(ListValue());
            isFirstTrailer = false;
            continue;
        }

        // Check if funcName refers to a variable holding a FunctionValue (first-class function)
        // This allows: f = double; f(5)  or  apply(double, 5)
        {
            Value* varPtr = nullptr;
            if (localVariables != nullptr) {
                auto lit = localVariables->find(funcName);
                if (lit != localVariables->end()) varPtr = &lit->second;
            }
            if (!varPtr && enclosingLocalVariables != nullptr) {
                auto eit = enclosingLocalVariables->find(funcName);
                if (eit != enclosingLocalVariables->end()) varPtr = &eit->second;
            }
            if (!varPtr) {
                auto git = variables.find(funcName);
                if (git != variables.end()) varPtr = &git->second;
            }
            if (varPtr && std::holds_alternative<FunctionValue>(*varPtr)) {
                const FunctionValue& fv = std::get<FunctionValue>(*varPtr);
                funcName = fv.name;  // Use the actual function name for lookup
                // (capturedLocals will be injected below after we find the FunctionDef)
                // Store fv captured locals to inject
                // We'll handle this inline below by storing a local copy
                const std::map<std::string, Value> capturedLocals = fv.capturedLocals;
                
                auto funcIt2 = functions.find(funcName);
                if (funcIt2 != functions.end()) {
                    const FunctionDef& funcDef = funcIt2->second;
                    
                    std::vector<Value> positionalArgs;
                    std::map<std::string, Value> keywordArgs;
                    bool seenKeyword = false;
                    
                    auto arglist = trailer->arglist();
                    if (arglist) {
                        auto args = arglist->argument();
                        for (auto arg : args) {
                            auto tests = arg->test();
                            if (tests.size() == 2) {
                                seenKeyword = true;
                                std::string paramName = tests[0]->getText();
                                auto argValue = visit(tests[1]);
                                Value val = Value(std::monostate{});
                                if (argValue.has_value()) {
                                    try { val = std::any_cast<Value>(argValue); } catch (...) {}
                                }
                                if (keywordArgs.find(paramName) != keywordArgs.end()) {
                                    throw std::runtime_error("Duplicate keyword argument: " + paramName);
                                }
                                keywordArgs[paramName] = val;
                            } else if (!tests.empty()) {
                                if (seenKeyword) {
                                    throw std::runtime_error("Positional argument follows keyword argument");
                                }
                                auto argValue = visit(tests[0]);
                                Value val = Value(std::monostate{});
                                if (argValue.has_value()) {
                                    try { val = std::any_cast<Value>(argValue); } catch (...) {}
                                }
                                positionalArgs.push_back(val);
                            }
                        }
                    }
                    
                    std::map<std::string, Value> localVars;
                    std::map<std::string, Value>* savedLocalVariables = localVariables;
                    const std::set<std::string>* savedFunctionLocals = currentFunctionLocals;
                    std::set<std::string> savedFunctionGlobals = currentFunctionGlobals;
                    std::map<std::string, Value>* savedEnclosingLocalVariables = enclosingLocalVariables;
                    
                    // Inject captured locals for closure support (before binding params)
                    for (const auto& [k, v] : capturedLocals) {
                        localVars[k] = v;
                    }
                    // Inject the function itself for recursive closure calls (allows self-recursion at any depth)
                    if (!capturedLocals.empty()) {
                        localVars[funcName] = Value(FunctionValue(funcName, capturedLocals));
                    }
                    
                    enclosingLocalVariables = savedLocalVariables;
                    localVariables = &localVars;
                    currentFunctionLocals = &funcDef.assignedVars;
                    currentFunctionGlobals = funcDef.globalVars;
                    
                    std::set<std::string> boundParams;
                    
                    if (positionalArgs.size() > funcDef.parameters.size()) {
                        localVariables = savedLocalVariables;
                        currentFunctionLocals = savedFunctionLocals;
                        currentFunctionGlobals = savedFunctionGlobals;
                        enclosingLocalVariables = savedEnclosingLocalVariables;
                        throw std::runtime_error("Too many positional arguments");
                    }
                    
                    // Bind positional args (overrides captured locals for params)
                    for (size_t pi = 0; pi < positionalArgs.size(); pi++) {
                        localVars[funcDef.parameters[pi]] = positionalArgs[pi];
                        boundParams.insert(funcDef.parameters[pi]);
                    }
                    
                    for (const auto& kwarg : keywordArgs) {
                        const std::string& paramName = kwarg.first;
                        auto it = std::find(funcDef.parameters.begin(), funcDef.parameters.end(), paramName);
                        if (it == funcDef.parameters.end()) {
                            localVariables = savedLocalVariables;
                            currentFunctionLocals = savedFunctionLocals;
                            currentFunctionGlobals = savedFunctionGlobals;
                            enclosingLocalVariables = savedEnclosingLocalVariables;
                            throw std::runtime_error("Unknown parameter name: " + paramName);
                        }
                        if (boundParams.find(paramName) != boundParams.end()) {
                            localVariables = savedLocalVariables;
                            currentFunctionLocals = savedFunctionLocals;
                            currentFunctionGlobals = savedFunctionGlobals;
                            enclosingLocalVariables = savedEnclosingLocalVariables;
                            throw std::runtime_error("Parameter " + paramName + " specified multiple times");
                        }
                        localVars[paramName] = kwarg.second;
                        boundParams.insert(paramName);
                    }
                    
                    // Use captured defaults from FunctionValue if available
                    const std::vector<Value>& effectiveDefaults2 = fv.hasOwnDefaults ? fv.capturedDefaults : funcDef.defaultValues;
                    
                    for (size_t pi = 0; pi < funcDef.parameters.size(); pi++) {
                        const std::string& paramName = funcDef.parameters[pi];
                        if (boundParams.find(paramName) == boundParams.end()) {
                            // Check if this parameter has a default value
                            // Parameters with defaults are the last numDefaultParams params
                            if (pi >= funcDef.parameters.size() - funcDef.numDefaultParams) {
                                localVars[paramName] = effectiveDefaults2[pi];
                            } else {
                                localVariables = savedLocalVariables;
                                currentFunctionLocals = savedFunctionLocals;
                                currentFunctionGlobals = savedFunctionGlobals;
                                enclosingLocalVariables = savedEnclosingLocalVariables;
                                throw std::runtime_error("Missing required parameter: " + paramName);
                            }
                        }
                    }
                    
                    Value returnValue = Value(std::monostate{});
                    try {
                        visit(funcDef.body);
                    } catch (const ReturnException& e) {
                        returnValue = e.returnValue;
                    }
                    
                    localVariables = savedLocalVariables;
                    currentFunctionLocals = savedFunctionLocals;
                    currentFunctionGlobals = savedFunctionGlobals;
                    enclosingLocalVariables = savedEnclosingLocalVariables;
                    
                    currentValue = returnValue;
                    isFirstTrailer = false;
                    continue;
                }
            }
        }

        // Check if this is a user-defined function
        auto funcIt = functions.find(funcName);
        if (funcIt != functions.end()) {
            // This is a user-defined function call
            const FunctionDef& funcDef = funcIt->second;
            
            // Evaluate the arguments - support both positional and keyword arguments
            std::vector<Value> positionalArgs;
            std::map<std::string, Value> keywordArgs;
            bool seenKeyword = false;
            
            auto arglist = trailer->arglist();
            if (arglist) {
                auto args = arglist->argument();
                for (auto arg : args) {
                    auto tests = arg->test();
                    
                    // Check if this is a keyword argument: test '=' test
                    if (tests.size() == 2) {
                        // This is a keyword argument
                        seenKeyword = true;
                        
                        // The first test must be a simple name (identifier)
                        std::string paramName = tests[0]->getText();
                        
                        // Evaluate the value (second test)
                        auto argValue = visit(tests[1]);
                        Value val = Value(std::monostate{});
                        if (argValue.has_value()) {
                            try {
                                val = std::any_cast<Value>(argValue);
                            } catch (...) {
                                // Keep as None
                            }
                        }
                        
                        // Store the keyword argument
                        if (keywordArgs.find(paramName) != keywordArgs.end()) {
                            throw std::runtime_error("Duplicate keyword argument: " + paramName);
                        }
                        keywordArgs[paramName] = val;
                    } else if (!tests.empty()) {
                        // This is a positional argument
                        if (seenKeyword) {
                            throw std::runtime_error("Positional argument follows keyword argument");
                        }
                        
                        auto argValue = visit(tests[0]);
                        Value val = Value(std::monostate{});
                        if (argValue.has_value()) {
                            try {
                                val = std::any_cast<Value>(argValue);
                            } catch (...) {
                                // Keep as None
                            }
                        }
                        positionalArgs.push_back(val);
                    }
                }
            }
            
            // Create local variable scope
            std::map<std::string, Value> localVars;
            std::map<std::string, Value>* savedLocalVariables = localVariables;
            const std::set<std::string>* savedFunctionLocals = currentFunctionLocals;
            std::set<std::string> savedFunctionGlobals = currentFunctionGlobals;
            std::map<std::string, Value>* savedEnclosingLocalVariables = enclosingLocalVariables;
            
            // The current function's locals become the enclosing scope for nested calls
            enclosingLocalVariables = savedLocalVariables;
            localVariables = &localVars;
            currentFunctionLocals = &funcDef.assignedVars;
            currentFunctionGlobals = funcDef.globalVars;
            
            // Argument Matching Algorithm:
            // 1. Initialize all parameters to None
            // 2. Bind positional arguments first (left-to-right)
            // 3. Bind keyword arguments (by name)
            // 4. Apply default values for unmatched parameters
            // 5. Error if any required parameter is still unmatched
            
            std::set<std::string> boundParams;
            
            // Step 1 & 2: Bind positional arguments
            if (positionalArgs.size() > funcDef.parameters.size()) {
                throw std::runtime_error("Too many positional arguments");
            }
            
            for (size_t i = 0; i < positionalArgs.size(); i++) {
                localVars[funcDef.parameters[i]] = positionalArgs[i];
                boundParams.insert(funcDef.parameters[i]);
            }
            
            // Step 3: Bind keyword arguments
            for (const auto& kwarg : keywordArgs) {
                const std::string& paramName = kwarg.first;
                const Value& argValue = kwarg.second;
                
                // Check if parameter exists
                auto it = std::find(funcDef.parameters.begin(), funcDef.parameters.end(), paramName);
                if (it == funcDef.parameters.end()) {
                    throw std::runtime_error("Unknown parameter name: " + paramName);
                }
                
                // Check if already bound by positional argument
                if (boundParams.find(paramName) != boundParams.end()) {
                    throw std::runtime_error("Parameter " + paramName + " specified multiple times");
                }
                
                localVars[paramName] = argValue;
                boundParams.insert(paramName);
            }
            
            // Step 4 & 5: Apply defaults or error for unmatched required parameters
            // Check if there's a FunctionValue with captured defaults for this function
            const std::vector<Value>* effectiveDefaults3 = &funcDef.defaultValues;
            {
                Value* fvPtr = nullptr;
                if (localVariables) {
                    auto lit = localVariables->find(funcName);
                    if (lit != localVariables->end()) fvPtr = &lit->second;
                }
                if (!fvPtr) {
                    auto git = variables.find(funcName);
                    if (git != variables.end()) fvPtr = &git->second;
                }
                if (fvPtr && std::holds_alternative<FunctionValue>(*fvPtr)) {
                    const FunctionValue& fv3 = std::get<FunctionValue>(*fvPtr);
                    if (fv3.hasOwnDefaults) {
                        effectiveDefaults3 = &fv3.capturedDefaults;
                    }
                }
            }
            for (size_t i = 0; i < funcDef.parameters.size(); i++) {
                const std::string& paramName = funcDef.parameters[i];
                
                if (boundParams.find(paramName) == boundParams.end()) {
                    // Parameter not bound - try to use default
                    // Parameters with defaults are the last numDefaultParams params
                    if (i >= funcDef.parameters.size() - funcDef.numDefaultParams) {
                        localVars[paramName] = (*effectiveDefaults3)[i];
                    } else {
                        // No default value - this is an error
                        throw std::runtime_error("Missing required parameter: " + paramName);
                    }
                }
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
            enclosingLocalVariables = savedEnclosingLocalVariables;
            
            // Store the return value and continue (allow subscript trailers after function call)
            currentValue = returnValue;
            isFirstTrailer = false;
            continue;
        }
        
        currentValue = std::any();
        isFirstTrailer = false;
        }
    }
    
    // After processing all trailers, return the current value
    // If no trailers, currentValue will be unset, so return the atom value
    if (!currentValue.has_value()) {
        return visit(atom);
    }
    return currentValue;
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
        // Concatenate all adjacent string literals (Python allows "foo" "bar" == "foobar")
        std::string strValue;
        for (auto s : strings) {
            strValue += unquoteString(s->getText());
        }
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
            // Check enclosing scope (for nested functions)
            if (enclosingLocalVariables != nullptr) {
                auto encIt = enclosingLocalVariables->find(varName);
                if (encIt != enclosingLocalVariables->end()) {
                    return encIt->second;
                }
            }
            // Local variable not initialized - fall back to global scope per spec:
            // "global variables are effective in all scopes (can be accessed without the global keyword)"
            auto globalIt = variables.find(varName);
            if (globalIt != variables.end()) {
                return globalIt->second;
            }
            return Value(std::monostate{});  // Variable not found anywhere
        }
        // Otherwise, check global variables (includes parameters which are in local scope but not in assignedVars, and globals)
        if (!isGlobal && localVariables != nullptr) {
            auto localIt = localVariables->find(varName);
            if (localIt != localVariables->end()) {
                return localIt->second;
            }
        }
        // Check enclosing scope (for nested functions)
        if (enclosingLocalVariables != nullptr) {
            auto encIt = enclosingLocalVariables->find(varName);
            if (encIt != enclosingLocalVariables->end()) {
                return encIt->second;
            }
        }
        auto it = variables.find(varName);
        if (it != variables.end()) {
            return it->second;
        }
        // Variable not found in variables — check if it's a function name
        if (functions.find(varName) != functions.end()) {
            return Value(FunctionValue(varName));
        }
        // Not found anywhere, return None
        return Value(std::monostate{});
    }
    
    // Check if this is a list literal: '[' testlist? ']'
    if (ctx->OPEN_BRACK()) {
        // This is a list literal
        std::vector<Value> elements;
        auto testlist = ctx->testlist();
        if (testlist) {
            auto tests = testlist->test();
            for (auto test : tests) {
                auto result = visit(test);
                if (result.has_value()) {
                    try {
                        elements.push_back(std::any_cast<Value>(result));
                    } catch (...) {
                        elements.push_back(Value(std::monostate{}));
                    }
                } else {
                    elements.push_back(Value(std::monostate{}));
                }
            }
        }
        // Return list (empty if no testlist)
        return Value(ListValue(elements));
    }
    
    // Check if this is a parenthesized expression or tuple: '(' testlist? ')'
    if (ctx->OPEN_PAREN()) {
        auto testlist = ctx->testlist();
        if (!testlist) {
            // Empty parens: () = empty tuple
            return Value(TupleValue(std::vector<Value>{}));
        }
        auto tests = testlist->test();
        // Check for trailing comma (distinguishes (x,) single-element tuple from (x) grouping)
        std::string testlistText = testlist->getText();
        bool hasTrailingComma = !testlistText.empty() && testlistText.back() == ',';
        
        if (tests.size() == 1 && !hasTrailingComma) {
            // Single element with no trailing comma: (x) is just grouping, return x
            return visit(tests[0]);
        } else {
            // Multiple elements OR trailing comma: create a tuple
            std::vector<Value> elements;
            for (auto t : tests) {
                auto result = visit(t);
                if (result.has_value()) {
                    try {
                        elements.push_back(std::any_cast<Value>(result));
                    } catch (...) {
                        elements.push_back(Value(std::monostate{}));
                    }
                } else {
                    elements.push_back(Value(std::monostate{}));
                }
            }
            return Value(TupleValue(elements));
        }
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
                    // F-strings use Python repr style for floats (1.0 not 1.000000) per test14
                    if (std::holds_alternative<double>(val)) {
                        result += floatToRepr(std::get<double>(val));
                    } else {
                        result += valueToString(val);
                    }
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
        
        // Promote bool to int for arithmetic (Python: bool IS-A int, True=1, False=0)
        if (std::holds_alternative<bool>(result)) {
            result = Value(std::get<bool>(result) ? 1 : 0);
        }
        if (std::holds_alternative<bool>(term)) {
            term = Value(std::get<bool>(term) ? 1 : 0);
        }
        
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
                result = tryDowncastBigInteger(left + right);
            } else if (op == "-") {
                result = tryDowncastBigInteger(left - right);
            }
        } else if (std::holds_alternative<int>(result) && std::holds_alternative<int>(term)) {
            // int op int -> int
            int left = std::get<int>(result);
            int right = std::get<int>(term);
            if (op == "+") {
                // Check for overflow BEFORE addition
                if (willOverflowAdd(left, right)) {
                    // Promote to BigInteger to handle large result
                    result = tryDowncastBigInteger(BigInteger(left) + BigInteger(right));
                } else {
                    result = left + right;
                }
            } else if (op == "-") {
                // Check for overflow BEFORE subtraction
                if (willOverflowSubtract(left, right)) {
                    // Promote to BigInteger to handle large result
                    result = tryDowncastBigInteger(BigInteger(left) - BigInteger(right));
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
        } else if (std::holds_alternative<ListValue>(result) && std::holds_alternative<ListValue>(term)) {
            // List concatenation: [1,2] + [3,4] = [1,2,3,4] (creates NEW list)
            if (op == "+") {
                std::vector<Value> newElems = *std::get<ListValue>(result).elements;
                const auto& rhs = *std::get<ListValue>(term).elements;
                newElems.insert(newElems.end(), rhs.begin(), rhs.end());
                result = ListValue(newElems);
            }
        } else if (std::holds_alternative<TupleValue>(result) && std::holds_alternative<TupleValue>(term)) {
            // Tuple concatenation: (1, 2) + (3, 4) = (1, 2, 3, 4)
            if (op == "+") {
                std::vector<Value> newElems = std::get<TupleValue>(result).elements;
                const auto& rhs = std::get<TupleValue>(term).elements;
                newElems.insert(newElems.end(), rhs.begin(), rhs.end());
                result = TupleValue(newElems);
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
        
        // Promote bool to int for arithmetic (Python: bool IS-A int, True=1, False=0)
        if (std::holds_alternative<bool>(result)) {
            result = Value(std::get<bool>(result) ? 1 : 0);
        }
        if (std::holds_alternative<bool>(factor)) {
            factor = Value(std::get<bool>(factor) ? 1 : 0);
        }
        
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
        } else if (op == "*" && ((std::holds_alternative<ListValue>(result) && std::holds_alternative<int>(factor)) ||
                                  (std::holds_alternative<int>(result) && std::holds_alternative<ListValue>(factor)))) {
            // List repetition: [0]*3 = [0, 0, 0]
            ListValue baseList;
            int count;
            if (std::holds_alternative<ListValue>(result)) {
                baseList = std::get<ListValue>(result);
                count = std::get<int>(factor);
            } else {
                count = std::get<int>(result);
                baseList = std::get<ListValue>(factor);
            }
            ListValue newList;
            if (count > 0) {
                newList.elements->reserve(baseList.elements->size() * count);
                for (int i = 0; i < count; i++) {
                    newList.elements->insert(newList.elements->end(), 
                                            baseList.elements->begin(), baseList.elements->end());
                }
            }
            result = newList;
        } else if (op == "*" && ((std::holds_alternative<TupleValue>(result) && std::holds_alternative<int>(factor)) ||
                                  (std::holds_alternative<int>(result) && std::holds_alternative<TupleValue>(factor)))) {
            // Tuple repetition: (1, 2) * 3 = (1, 2, 1, 2, 1, 2)
            TupleValue baseTuple;
            int count;
            if (std::holds_alternative<TupleValue>(result)) {
                baseTuple = std::get<TupleValue>(result);
                count = std::get<int>(factor);
            } else {
                count = std::get<int>(result);
                baseTuple = std::get<TupleValue>(factor);
            }
            std::vector<Value> newElems;
            if (count > 0) {
                newElems.reserve(baseTuple.elements.size() * count);
                for (int i = 0; i < count; i++) {
                    newElems.insert(newElems.end(), baseTuple.elements.begin(), baseTuple.elements.end());
                }
            }
            result = TupleValue(newElems);
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
                result = tryDowncastBigInteger(left * right);
            } else if (op == "//") {
                result = tryDowncastBigInteger(left.floorDiv(right));
            } else if (op == "%") {
                result = tryDowncastBigInteger(left % right);
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
                    result = tryDowncastBigInteger(BigInteger(left) * BigInteger(right));
                } else {
                    result = left * right;
                }
            } else if (op == "//") {
                // Check for overflow BEFORE floor division
                if (willOverflowFloorDiv(left, right)) {
                    // Promote to BigInteger
                    result = tryDowncastBigInteger(BigInteger(left).floorDiv(BigInteger(right)));
                } else {
                    result = pythonFloorDiv(left, right);
                }
            } else if (op == "%") {
                // Check for overflow BEFORE modulo (though it rarely overflows)
                if (willOverflowModulo(left, right)) {
                    // Promote to BigInteger
                    result = tryDowncastBigInteger(BigInteger(left) % BigInteger(right));
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
                // Float floor division: floor toward -infinity, result is float
                result = std::floor(left / right);
            } else if (op == "%") {
                // Float modulo: a - floor(a/b)*b, result is float
                result = left - std::floor(left / right) * right;
            }
        }
    }
    
    return result;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    // factor: ('+'|'-') factor | power
    
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
                if (std::holds_alternative<bool>(factorVal)) {
                    // bool IS-A int: -True = -1, -False = 0
                    bool b = std::get<bool>(factorVal);
                    return Value(b ? -1 : 0);
                } else if (std::holds_alternative<int>(factorVal)) {
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
                // Unary plus: +True = 1 (int), +False = 0 (int)
                if (std::holds_alternative<bool>(factorVal)) {
                    bool b = std::get<bool>(factorVal);
                    return Value(b ? 1 : 0);
                }
                return factorVal;
            }
        }
    } else {
        // This is 'power' rule
        auto powerCtx = ctx->power();
        if (powerCtx) {
            return visit(powerCtx);
        }
    }
    
    return std::any();
}

std::any EvalVisitor::visitPower(Python3Parser::PowerContext *ctx) {
    // power: atom_expr (POWER factor)?
    auto atomExpr = ctx->atom_expr();
    if (!atomExpr) {
        return std::any();
    }
    
    auto baseAny = visit(atomExpr);
    
    // Check if there's a ** (power) operator
    auto factorCtx = ctx->factor();
    if (factorCtx) {
        // This is atom_expr ** factor
        auto expAny = visit(factorCtx);
        Value base, exp;
        try {
            base = std::any_cast<Value>(baseAny);
            exp = std::any_cast<Value>(expAny);
        } catch (...) {
            return Value(0);
        }
        return powerValue(base, exp);
    }
    
    return baseAny;
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
        } else if ((std::holds_alternative<bool>(left) || std::holds_alternative<int>(left)) &&
                   (std::holds_alternative<bool>(right) || std::holds_alternative<int>(right))) {
            // bool vs int or int vs bool: In Python, bool is a subclass of int (True=1, False=0)
            int l = std::holds_alternative<bool>(left) ? (std::get<bool>(left) ? 1 : 0) : std::get<int>(left);
            int r = std::holds_alternative<bool>(right) ? (std::get<bool>(right) ? 1 : 0) : std::get<int>(right);
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if ((std::holds_alternative<double>(left) || std::holds_alternative<bool>(left)) &&
                   (std::holds_alternative<double>(right) || std::holds_alternative<bool>(right))) {
            // double vs bool, bool vs double: convert bool to double (True=1.0, False=0.0)
            double l = std::holds_alternative<double>(left) ? std::get<double>(left) : (std::get<bool>(left) ? 1.0 : 0.0);
            double r = std::holds_alternative<double>(right) ? std::get<double>(right) : (std::get<bool>(right) ? 1.0 : 0.0);
            if (op == "<") compResult = l < r;
            else if (op == ">") compResult = l > r;
            else if (op == "<=") compResult = l <= r;
            else if (op == ">=") compResult = l >= r;
            else if (op == "==") compResult = l == r;
            else if (op == "!=") compResult = l != r;
        } else if (std::holds_alternative<TupleValue>(left) && std::holds_alternative<TupleValue>(right)) {
            // Tuple lexicographic comparison
            int c = compareValues(left, right);
            if (op == "==") compResult = (c == 0);
            else if (op == "!=") compResult = (c != 0);
            else if (op == "<")  compResult = (c < 0);
            else if (op == ">")  compResult = (c > 0);
            else if (op == "<=") compResult = (c <= 0);
            else if (op == ">=") compResult = (c >= 0);
            else compResult = false;
        } else if (std::holds_alternative<ListValue>(left) && std::holds_alternative<ListValue>(right)) {
            // List lexicographic comparison
            int c = compareValues(left, right);
            if (op == "==") compResult = (c == 0);
            else if (op == "!=") compResult = (c != 0);
            else if (op == "<")  compResult = (c < 0);
            else if (op == ">")  compResult = (c > 0);
            else if (op == "<=") compResult = (c <= 0);
            else if (op == ">=") compResult = (c >= 0);
            else compResult = false;
        } else if (std::holds_alternative<std::monostate>(left) && std::holds_alternative<std::monostate>(right)) {
            // None vs None: None == None is True, None != None is False
            if (op == "==") compResult = true;
            else if (op == "!=") compResult = false;
            else compResult = false;  // None is not < > <= >= anything
        } else {
            // Different types - use equality/inequality for == and !=, false for ordering
            if (op == "==") compResult = false;
            else if (op == "!=") compResult = true;
            else compResult = false;
        }
        
        finalResult = finalResult && compResult;
        if (!finalResult) break;  // Short-circuit: once false, chained result stays false
        
        // For chained comparisons, the right becomes the new left
        left = right;
    }
    
    return Value(finalResult);
}

std::string EvalVisitor::unquoteString(const std::string& str) {
    // Remove surrounding quotes from string literals and process escape sequences
    if (str.length() >= 2) {
        if ((str.front() == '"' && str.back() == '"') ||
            (str.front() == '\'' && str.back() == '\'')) {
            std::string inner = str.substr(1, str.length() - 2);
            // Process escape sequences
            std::string result;
            result.reserve(inner.size());
            for (size_t i = 0; i < inner.size(); ++i) {
                if (inner[i] == '\\' && i + 1 < inner.size()) {
                    char next = inner[i + 1];
                    switch (next) {
                        case 'n':  result += '\n'; ++i; break;
                        case 't':  result += '\t'; ++i; break;
                        case 'r':  result += '\r'; ++i; break;
                        case '\\': result += '\\'; ++i; break;
                        case '\'': result += '\''; ++i; break;
                        case '"':  result += '"';  ++i; break;
                        case '0':  result += '\0'; ++i; break;
                        default:   result += inner[i]; break;
                    }
                } else {
                    result += inner[i];
                }
            }
            return result;
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
        return oss.str();
    } else if (std::holds_alternative<bool>(val)) {
        // Python prints True/False, not 1/0
        return std::get<bool>(val) ? "True" : "False";
    } else if (std::holds_alternative<std::monostate>(val)) {
        // Python prints None
        return "None";
    } else if (std::holds_alternative<BigInteger>(val)) {
        // Print BigInteger
        return std::get<BigInteger>(val).toString();
    } else if (std::holds_alternative<TupleValue>(val)) {
        // Print tuple as (elem1, elem2, ...) with repr for elements
        const TupleValue& tuple = std::get<TupleValue>(val);
        std::string result = "(";
        for (size_t i = 0; i < tuple.elements.size(); i++) {
            result += valueToRepr(tuple.elements[i]);
            if (i < tuple.elements.size() - 1) {
                result += ", ";
            }
        }
        // Special case: single-element tuple needs trailing comma
        if (tuple.elements.size() == 1) {
            result += ",";
        }
        result += ")";
        return result;
    } else if (std::holds_alternative<ListValue>(val)) {
        // Print list as [elem1, elem2, ...] with repr for elements
        const ListValue& list = std::get<ListValue>(val);
        std::string result = "[";
        for (size_t i = 0; i < list.elements->size(); i++) {
            result += valueToRepr((*list.elements)[i]);
            if (i < list.elements->size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    } else if (std::holds_alternative<FunctionValue>(val)) {
        return "<function " + std::get<FunctionValue>(val).name + ">";
    }
    return "";
}

std::string EvalVisitor::floatToRepr(double d) {
    // Python-style float repr: shortest representation that uniquely identifies the double
    // Python uses %r which for most practical purposes is like:
    // - Show the shortest decimal that round-trips back to the same double
    // - Use fixed notation (no scientific) for most values
    // - Always show at least one decimal digit after the point
    // Examples: 1.0 → "1.0", 3.14 → "3.14", 0.1 → "0.1", 100.0 → "100.0"
    
    // Handle special cases
    if (std::isinf(d)) return d > 0 ? "inf" : "-inf";
    if (std::isnan(d)) return "nan";
    
    // Try increasing precision to find shortest roundtrip
    for (int prec = 1; prec <= 17; prec++) {
        // Use %g-style formatting (shortest of %f and %e)
        char buf[64];
        snprintf(buf, sizeof(buf), "%.*g", prec, d);
        std::string s(buf);
        
        // Check if roundtrips correctly
        double parsed = std::strtod(s.c_str(), nullptr);
        if (parsed == d) {
            // Python prefers non-scientific notation for most values
            // If we got scientific notation, try fixed notation
            if (s.find('e') != std::string::npos || s.find('E') != std::string::npos) {
                // Try with enough decimal places in fixed notation
                // Python avoids scientific notation for values between 1e-4 and 1e16
                double absVal = std::fabs(d);
                if (absVal >= 1e-4 && absVal < 1e16) {
                    // Use fixed notation
                    char buf2[256];
                    snprintf(buf2, sizeof(buf2), "%.17f", d);
                    std::string fixed(buf2);
                    // Remove trailing zeros, but keep at least one decimal digit
                    size_t dot = fixed.find('.');
                    if (dot != std::string::npos) {
                        size_t last_nonzero = fixed.find_last_not_of('0');
                        if (last_nonzero != std::string::npos && last_nonzero > dot) {
                            fixed = fixed.substr(0, last_nonzero + 1);
                        } else {
                            // Keep at least x.0
                            fixed = fixed.substr(0, dot + 2);
                        }
                    }
                    // Verify roundtrip
                    double parsed2 = std::strtod(fixed.c_str(), nullptr);
                    if (parsed2 == d) {
                        return fixed;
                    }
                }
                // Keep scientific notation if we can't use fixed
                return s;
            }
            // Ensure there's a decimal point
            if (s.find('.') == std::string::npos) {
                s += ".0";
            }
            return s;
        }
    }
    
    // Fallback: full precision
    char buf[64];
    snprintf(buf, sizeof(buf), "%.17g", d);
    std::string s(buf);
    if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
        s += ".0";
    }
    return s;
}

Value EvalVisitor::tryDowncastBigInteger(const BigInteger& bi) {
    // If the BigInteger fits in a regular int, downcast it for performance
    if (bi.fitsInInt()) {
        return Value(static_cast<int>(bi.toLongLong()));
    }
    return Value(bi);
}

std::string EvalVisitor::valueToRepr(const Value& val) {
    // Returns the repr of a value (strings are quoted, floats use Python repr, containers recurse)
    if (std::holds_alternative<std::string>(val)) {
        // Strings get quoted with single quotes in repr
        return "'" + std::get<std::string>(val) + "'";
    } else if (std::holds_alternative<double>(val)) {
        // Floats inside containers use Python-style repr (e.g., 1.0 not 1.000000)
        return floatToRepr(std::get<double>(val));
    } else if (std::holds_alternative<TupleValue>(val)) {
        // Recurse for tuple elements
        return valueToString(val);
    } else if (std::holds_alternative<ListValue>(val)) {
        // Recurse for list elements
        return valueToString(val);
    } else {
        // For all other types, repr == str
        return valueToString(val);
    }
}

Python3Parser::Atom_exprContext* EvalVisitor::getAtomExprFromTest(Python3Parser::TestContext* test) {
    // Navigate the grammar tree: test -> or_test -> and_test -> not_test -> comparison -> arith_expr -> term -> factor -> power -> atom_expr
    if (!test) return nullptr;
    auto orTest = test->or_test();
    if (!orTest) return nullptr;
    auto andTests = orTest->and_test();
    if (andTests.size() != 1) return nullptr;
    auto notTests = andTests[0]->not_test();
    if (notTests.size() != 1) return nullptr;
    auto notTest = notTests[0];
    if (notTest->not_test()) return nullptr; // has 'not' prefix
    auto comparison = notTest->comparison();
    if (!comparison) return nullptr;
    auto arithExprs = comparison->arith_expr();
    if (arithExprs.size() != 1) return nullptr;
    auto terms = arithExprs[0]->term();
    if (terms.size() != 1) return nullptr;
    auto factors = terms[0]->factor();
    if (factors.size() != 1) return nullptr;
    auto factor = factors[0];
    if (factor->factor()) return nullptr; // has unary prefix
    auto power = factor->power();
    if (!power) return nullptr;
    if (power->factor()) return nullptr; // has ** operator
    return power->atom_expr();
}

bool EvalVisitor::valueToBool(const Value& val) {
    // Convert Python value to bool using Python's truthiness rules
    // False values: None, False, 0, 0.0, empty string "", BigInteger(0), empty tuple ()
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
    } else if (std::holds_alternative<TupleValue>(val)) {
        return !std::get<TupleValue>(val).elements.empty();
    } else if (std::holds_alternative<ListValue>(val)) {
        return !std::get<ListValue>(val).elements->empty();
    } else if (std::holds_alternative<FunctionValue>(val)) {
        return true; // Functions are always truthy
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
    std::vector<Value> defaults;
    size_t numDefaultParams = 0;  // Track number of parameters with defaults
    
    if (parametersCtx) {
        // parameters: '(' typedargslist? ')'
        auto typedargslist = parametersCtx->typedargslist();
        if (typedargslist) {
            // typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
            // tfpdef: NAME
            auto tfpdefs = typedargslist->tfpdef();
            auto tests = typedargslist->test();  // Default value expressions
            
            // Parse parameter names
            for (auto tfpdef : tfpdefs) {
                auto paramName = tfpdef->NAME();
                if (paramName) {
                    params.push_back(paramName->getText());
                }
            }
            
            // Calculate default values
            // The number of tests equals the number of parameters with defaults
            // Default parameters must be at the end, so:
            // - First (params.size() - tests.size()) parameters have no default (None)
            // - Last tests.size() parameters have defaults from tests
            size_t numParams = params.size();
            size_t numDefaults = tests.size();
            numDefaultParams = numDefaults;  // Save for funcDef
            size_t numNonDefaults = numParams - numDefaults;
            
            // Add None for parameters without defaults
            for (size_t i = 0; i < numNonDefaults; i++) {
                defaults.push_back(Value(std::monostate{}));  // None
            }
            
            // Add evaluated default expressions for parameters with defaults
            for (size_t i = 0; i < numDefaults; i++) {
                auto defaultExpr = visit(tests[i]);
                if (defaultExpr.has_value()) {
                    defaults.push_back(std::any_cast<Value>(defaultExpr));
                } else {
                    defaults.push_back(Value(std::monostate{}));  // None (e.g., explicit None default)
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
    funcDef.defaultValues = defaults;
    funcDef.numDefaultParams = numDefaultParams;
    funcDef.body = suite;
    funcDef.assignedVars = assignedVars;
    funcDef.globalVars = globalVars;
    functions[funcName] = funcDef;
    
    // Also store the function as a first-class value so it can be passed around
    if (localVariables != nullptr) {
        // Inside a function: capture current locals for closure support
        std::map<std::string, Value> captured = *localVariables;
        if (enclosingLocalVariables != nullptr) {
            for (auto& [k, v] : *enclosingLocalVariables) {
                if (captured.find(k) == captured.end()) captured[k] = v;
            }
        }
        (*localVariables)[funcName] = Value(FunctionValue(funcName, captured, defaults));
    } else {
        // At global scope - store defaults for functions defined at global scope too
        variables[funcName] = Value(FunctionValue(funcName, {}, defaults));
    }
    
    return std::any();
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    // return_stmt: 'return' [testlist]
    // Evaluate the return expression (if present)
    Value returnValue = Value(std::monostate{});  // Default to None
    
    auto testlist = ctx->testlist();
    if (testlist) {
        auto tests = testlist->test();
        if (tests.size() == 1) {
            // Single return value
            auto result = visit(tests[0]);
            if (result.has_value()) {
                try {
                    returnValue = std::any_cast<Value>(result);
                } catch (...) {
                    // If cast fails, return None
                    returnValue = Value(std::monostate{});
                }
            }
        } else if (tests.size() > 1) {
            // Multiple return values - return as tuple
            std::vector<Value> elements;
            for (auto test : tests) {
                auto result = visit(test);
                if (result.has_value()) {
                    try {
                        elements.push_back(std::any_cast<Value>(result));
                    } catch (...) {
                        // If cast fails, add None to tuple
                        elements.push_back(Value(std::monostate{}));
                    }
                } else {
                    elements.push_back(Value(std::monostate{}));
                }
            }
            returnValue = Value(TupleValue(elements));
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

// Power operation: base ** exp
Value EvalVisitor::powerValue(const Value& base, const Value& exp) {
    // Convert bools to ints first
    Value b = base, e = exp;
    if (std::holds_alternative<bool>(b)) {
        b = Value(std::get<bool>(b) ? 1 : 0);
    }
    if (std::holds_alternative<bool>(e)) {
        e = Value(std::get<bool>(e) ? 1 : 0);
    }
    
    // float ** anything or anything ** float → float result
    auto toDouble = [](const Value& v) -> double {
        if (std::holds_alternative<int>(v)) return (double)std::get<int>(v);
        if (std::holds_alternative<double>(v)) return std::get<double>(v);
        if (std::holds_alternative<BigInteger>(v)) {
            std::string s = std::get<BigInteger>(v).toString();
            return std::stod(s);
        }
        return 0.0;
    };
    
    bool baseIsFloat = std::holds_alternative<double>(b);
    bool expIsFloat = std::holds_alternative<double>(e);
    
    if (baseIsFloat || expIsFloat) {
        double bd = toDouble(b);
        double ed = toDouble(e);
        return Value(std::pow(bd, ed));
    }
    
    // int ** int or BigInteger ** int
    // Get exp as integer
    long long expInt = 0;
    if (std::holds_alternative<int>(e)) {
        expInt = std::get<int>(e);
    } else if (std::holds_alternative<BigInteger>(e)) {
        // For very large exponents, this could be huge. Just convert to long long.
        std::string s = std::get<BigInteger>(e).toString();
        try { expInt = std::stoll(s); } catch (...) { expInt = 0; }
    }
    
    // Negative exponent → float result
    if (expInt < 0) {
        double bd = toDouble(b);
        return Value(std::pow(bd, (double)expInt));
    }
    
    // Non-negative integer exponent: result is BigInteger
    BigInteger baseBI;
    if (std::holds_alternative<int>(b)) {
        baseBI = BigInteger(std::get<int>(b));
    } else if (std::holds_alternative<BigInteger>(b)) {
        baseBI = std::get<BigInteger>(b);
    } else {
        return Value(0);
    }
    
    // Fast exponentiation
    BigInteger result(1);
    BigInteger curBase = baseBI;
    long long ex = expInt;
    while (ex > 0) {
        if (ex % 2 == 1) {
            result = result * curBase;
        }
        curBase = curBase * curBase;
        ex /= 2;
    }
    
    // Try to downcast to int if small enough
    std::string resStr = result.toString();
    bool isNeg = (resStr.size() > 0 && resStr[0] == '-');
    std::string absStr = isNeg ? resStr.substr(1) : resStr;
    if (absStr.length() < 10 || (absStr.length() == 10 && absStr <= "2147483647")) {
        try {
            int intVal = std::stoi(resStr);
            return Value(intVal);
        } catch (...) {}
    }
    return Value(result);
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

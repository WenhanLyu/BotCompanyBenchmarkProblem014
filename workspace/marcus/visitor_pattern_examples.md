# ANTLR Visitor Pattern - Practical Examples

## How the Visitor Pattern Works

### 1. Grammar Rule → Context Class

Grammar (Python3Parser.g4):
```
arith_expr: term (addorsub_op term)*;
term: factor (muldivmod_op factor)*;
```

Generated Context (Python3Parser.h):
```cpp
class Arith_exprContext : public ParserRuleContext {
    std::vector<TermContext*> term();      // Get all term nodes
    TermContext* term(size_t i);           // Get i-th term
    std::vector<Addorsub_opContext*> addorsub_op();
    Addorsub_opContext* addorsub_op(size_t i);
    // ...
};
```

### 2. Visitor Pattern

Base implementation (Python3ParserBaseVisitor.h):
```cpp
virtual std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    return visitChildren(ctx);  // Default: visit all children
}
```

Our implementation (Evalvisitor.cpp):
```cpp
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    // Start with first term
    Value result = std::any_cast<Value>(visit(ctx->term(0)));
    
    // Apply operators left-to-right
    auto terms = ctx->term();
    auto ops = ctx->addorsub_op();
    
    for (size_t i = 1; i < terms.size(); i++) {
        Value right = std::any_cast<Value>(visit(terms[i]));
        auto op = ops[i-1];
        
        if (op->getText() == "+") {
            result = add(result, right);
        } else {  // "-"
            result = subtract(result, right);
        }
    }
    
    return result;
}
```

## Example: Implementing Arithmetic Expression

### Grammar Structure
```
arith_expr
  └─ term ('+' term)*
       └─ factor ('*' factor)*
            └─ atom_expr
                 └─ atom (NUMBER, STRING, NAME, etc.)
```

### Implementation Steps

**Step 1: visitAtom (leaf nodes)**
```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // NUMBER
    if (ctx->NUMBER()) {
        std::string numStr = ctx->NUMBER()->getText();
        return Value(BigInteger(numStr));
    }
    
    // STRING
    if (!ctx->STRING().empty()) {
        std::string str = ctx->STRING(0)->getText();
        return Value(unquoteString(str));
    }
    
    // True/False
    if (ctx->TRUE()) return Value(true);
    if (ctx->FALSE()) return Value(false);
    
    // None
    if (ctx->NONE()) return Value(std::monostate{});
    
    // NAME (variable)
    if (ctx->NAME()) {
        std::string varName = ctx->NAME()->getText();
        return scopes.getVariable(varName);
    }
    
    // Parenthesized expression
    if (ctx->test()) {
        return visit(ctx->test());
    }
    
    return Value(std::monostate{});
}
```

**Step 2: visitFactor (handles unary minus)**
```cpp
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    // factor: ('+' | '-') factor | atom_expr
    
    if (ctx->factor()) {
        // Unary operator present
        Value val = std::any_cast<Value>(visit(ctx->factor()));
        
        if (ctx->children[0]->getText() == "-") {
            return negate(val);
        }
        return val;  // Unary +
    } else {
        // No unary operator, just atom_expr
        return visit(ctx->atom_expr());
    }
}
```

**Step 3: visitTerm (multiplication/division)**
```cpp
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    // term: factor (muldivmod_op factor)*
    
    Value result = std::any_cast<Value>(visit(ctx->factor(0)));
    
    auto factors = ctx->factor();
    auto ops = ctx->muldivmod_op();
    
    for (size_t i = 1; i < factors.size(); i++) {
        Value right = std::any_cast<Value>(visit(factors[i]));
        std::string opStr = ops[i-1]->getText();
        
        if (opStr == "*") {
            result = multiply(result, right);
        } else if (opStr == "/") {
            result = divide(result, right);
        } else if (opStr == "//") {
            result = floorDiv(result, right);
        } else if (opStr == "%") {
            result = modulo(result, right);
        }
    }
    
    return result;
}
```

**Step 4: visitArith_expr (addition/subtraction)**
```cpp
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    // arith_expr: term (addorsub_op term)*
    
    Value result = std::any_cast<Value>(visit(ctx->term(0)));
    
    auto terms = ctx->term();
    auto ops = ctx->addorsub_op();
    
    for (size_t i = 1; i < terms.size(); i++) {
        Value right = std::any_cast<Value>(visit(terms[i]));
        
        if (ops[i-1]->getText() == "+") {
            result = add(result, right);
        } else {
            result = subtract(result, right);
        }
    }
    
    return result;
}
```

## Example: Handling Function Calls

### Grammar
```
atom_expr: atom trailer?
trailer: '(' arglist? ')'
arglist: argument (',' argument)*
argument: test (NAME '=' test)?
```

### Implementation
```cpp
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    auto atom = ctx->atom();
    auto trailer = ctx->trailer();
    
    if (!trailer) {
        // Just an atom, no function call
        return visit(atom);
    }
    
    // This is a function call
    std::string funcName = atom->getText();
    
    // Parse arguments
    std::vector<Value> positionalArgs;
    std::unordered_map<std::string, Value> keywordArgs;
    
    if (trailer->arglist()) {
        auto arguments = trailer->arglist()->argument();
        
        for (auto arg : arguments) {
            auto tests = arg->test();
            Value argValue = std::any_cast<Value>(visit(tests[0]));
            
            if (arg->NAME()) {
                // Keyword argument: name=value
                std::string paramName = arg->NAME()->getText();
                keywordArgs[paramName] = argValue;
            } else {
                // Positional argument
                positionalArgs.push_back(argValue);
            }
        }
    }
    
    // Call the function
    if (funcName == "print") {
        return callPrint(positionalArgs);
    } else if (funcName == "int") {
        return callInt(positionalArgs[0]);
    } else if (funcName == "float") {
        return callFloat(positionalArgs[0]);
    } else if (funcName == "str") {
        return callStr(positionalArgs[0]);
    } else if (funcName == "bool") {
        return callBool(positionalArgs[0]);
    } else {
        // User-defined function
        return callFunction(funcName, positionalArgs, keywordArgs);
    }
}
```

## Example: Control Flow (If Statement)

### Grammar
```
if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ('else' ':' suite)?
```

### Implementation
```cpp
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    auto tests = ctx->test();
    auto suites = ctx->suite();
    
    // Try if condition
    Value condition = std::any_cast<Value>(visit(tests[0]));
    if (isTruthy(condition)) {
        return visit(suites[0]);
    }
    
    // Try elif conditions
    for (size_t i = 1; i < tests.size(); i++) {
        condition = std::any_cast<Value>(visit(tests[i]));
        if (isTruthy(condition)) {
            return visit(suites[i]);
        }
    }
    
    // Else clause (if exists)
    if (suites.size() > tests.size()) {
        return visit(suites.back());
    }
    
    return Value(std::monostate{});
}
```

## Example: While Loop with Break/Continue

### Implementation
```cpp
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    while (true) {
        // Evaluate condition
        Value condition = std::any_cast<Value>(visit(ctx->test()));
        if (!isTruthy(condition)) {
            break;
        }
        
        try {
            // Execute body
            visit(ctx->suite());
        } catch (const BreakException&) {
            break;
        } catch (const ContinueException&) {
            continue;
        }
        // ReturnException propagates up to caller
    }
    
    return Value(std::monostate{});
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
    throw BreakException();
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
    throw ContinueException();
}
```

## Example: Variable Assignment

### Grammar
```
expr_stmt: testlist (augassign testlist | ('=' testlist)+)
testlist: test (',' test)*
augassign: '+=' | '-=' | '*=' | '/=' | '//=' | '%='
```

### Implementation
```cpp
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    auto testlists = ctx->testlist();
    
    if (testlists.size() == 1) {
        // Just an expression, not assignment
        return visit(testlists[0]);
    }
    
    // Assignment or augmented assignment
    auto augassign = ctx->augassign();
    
    if (augassign) {
        // Augmented assignment: a += b
        auto lhs = testlists[0]->test(0);
        std::string varName = lhs->getText();  // Simplified
        
        Value current = scopes.getVariable(varName);
        Value rhs = std::any_cast<Value>(visit(testlists[1]->test(0)));
        
        std::string op = augassign->getText();
        Value result;
        
        if (op == "+=") result = add(current, rhs);
        else if (op == "-=") result = subtract(current, rhs);
        else if (op == "*=") result = multiply(current, rhs);
        else if (op == "/=") result = divide(current, rhs);
        else if (op == "//=") result = floorDiv(current, rhs);
        else if (op == "%=") result = modulo(current, rhs);
        
        scopes.setVariable(varName, result);
        return result;
    } else {
        // Regular assignment: a = b = c
        // Evaluate rightmost
        Value value = std::any_cast<Value>(visit(testlists.back()));
        
        // Assign to all targets (right-to-left)
        for (int i = testlists.size() - 2; i >= 0; i--) {
            auto tests = testlists[i]->test();
            for (auto test : tests) {
                std::string varName = test->getText();  // Simplified
                scopes.setVariable(varName, value);
            }
        }
        
        return value;
    }
}
```

## Common Patterns

### 1. Accessing Children
```cpp
// Terminal node (token)
auto token = ctx->NUMBER();
std::string text = token->getText();

// Non-terminal node (rule)
auto child = ctx->test();
Value result = std::any_cast<Value>(visit(child));

// Multiple children (vector)
auto children = ctx->test();
for (auto child : children) {
    visit(child);
}
```

### 2. Checking if Node Exists
```cpp
if (ctx->NUMBER()) {
    // NUMBER token exists
}

if (!ctx->test().empty()) {
    // At least one test node exists
}
```

### 3. Type Casting std::any
```cpp
// Safe cast
auto anyResult = visit(ctx->test());
if (anyResult.has_value()) {
    try {
        Value val = std::any_cast<Value>(anyResult);
        // Use val
    } catch (const std::bad_any_cast&) {
        // Handle type mismatch
    }
}

// Direct cast (if you're sure)
Value val = std::any_cast<Value>(visit(ctx->test()));
```

### 4. Returning Values
```cpp
// Return Value wrapped in std::any
return Value(42);
return Value(BigInteger("123"));
return Value(std::string("hello"));

// Return None
return Value(std::monostate{});
```

## Key Takeaways

1. **Use visit(), not visitXXX()** - The visitor pattern dispatches automatically
2. **Cast std::any to Value** - All visitor methods return std::any
3. **Check node existence** - Not all optional nodes are present
4. **Iterate carefully** - Lists are 0-indexed, operations are between adjacent elements
5. **Handle precedence** - The grammar already enforces precedence; just follow the tree
6. **Exceptions for control flow** - Clean way to handle break/continue/return

This pattern applies to all visitor methods - traverse the parse tree, evaluate children, combine results.

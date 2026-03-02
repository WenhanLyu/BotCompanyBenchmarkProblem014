# Python Interpreter Architecture Proposal

## Executive Summary

This document proposes a layered architecture for the Python interpreter with focus on:
1. **Value representation** using std::variant
2. **Visitor pattern** implementation extending ANTLR's BaseVisitor
3. **Scope management** for variables and functions
4. **Control flow** handling (break/continue/return)
5. **BigInteger** integration strategy

## 1. Core Value System

### 1.1 Value Type Definition

```cpp
// Forward declaration
class BigInteger;

// Python value types
using Value = std::variant<
    std::monostate,      // None
    bool,                // True/False
    BigInteger,          // Arbitrary precision integer
    double,              // Float (C++ double)
    std::string,         // String
    std::vector<Value>   // Tuple (nested variant)
>;
```

### 1.2 Type Checking Utilities

```cpp
bool isNone(const Value& v);
bool isBool(const Value& v);
bool isInt(const Value& v);
bool isFloat(const Value& v);
bool isString(const Value& v);
bool isTuple(const Value& v);

// Type conversions
Value toInt(const Value& v);
Value toFloat(const Value& v);
Value toString(const Value& v);
Value toBool(const Value& v);

// Truthiness (for if/while conditions)
bool isTruthy(const Value& v);
```

### 1.3 Operator Overloading

Instead of free functions, implement operators as methods or use std::visit:

```cpp
Value add(const Value& a, const Value& b);
Value subtract(const Value& a, const Value& b);
Value multiply(const Value& a, const Value& b);
Value divide(const Value& a, const Value& b);      // / operator
Value floorDiv(const Value& a, const Value& b);    // // operator
Value modulo(const Value& a, const Value& b);
Value negate(const Value& v);                      // unary -

// Comparison (returns bool wrapped in Value)
Value equal(const Value& a, const Value& b);
Value notEqual(const Value& a, const Value& b);
Value lessThan(const Value& a, const Value& b);
Value lessEqual(const Value& a, const Value& b);
Value greaterThan(const Value& a, const Value& b);
Value greaterEqual(const Value& a, const Value& b);
```

## 2. Scope Management

### 2.1 Variable Storage

```cpp
// Symbol table for variables
using SymbolTable = std::unordered_map<std::string, Value>;

// Scope stack (for function calls)
class ScopeManager {
private:
    std::vector<SymbolTable> scopes;  // Stack of scopes
    SymbolTable globalScope;           // Global variables
    
public:
    ScopeManager();
    
    // Scope operations
    void pushScope();                  // Enter function
    void popScope();                   // Exit function
    
    // Variable operations
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name);
    bool hasVariable(const std::string& name);
    
    // Special: Global scope accessible everywhere
    void setGlobal(const std::string& name, const Value& value);
    Value getGlobal(const std::string& name);
};
```

### 2.2 Function Storage

```cpp
struct FunctionDef {
    std::string name;
    std::vector<std::string> params;              // Parameter names
    std::unordered_map<std::string, Value> defaults;  // Default values
    Python3Parser::SuiteContext* body;            // Function body AST
};

class FunctionTable {
private:
    std::unordered_map<std::string, FunctionDef> functions;
    
public:
    void define(const FunctionDef& func);
    FunctionDef* get(const std::string& name);
    bool exists(const std::string& name);
};
```

## 3. Control Flow Handling

### 3.1 Exception-Based Control Flow

Use C++ exceptions for break/continue/return:

```cpp
// Control flow exceptions
struct BreakException {};
struct ContinueException {};
struct ReturnException {
    Value returnValue;
    ReturnException(const Value& v) : returnValue(v) {}
};
```

### 3.2 Usage in Visitor

```cpp
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    while (true) {
        // Evaluate condition
        auto condition = visit(ctx->test());
        if (!isTruthy(std::any_cast<Value>(condition))) {
            break;
        }
        
        try {
            // Execute body
            visit(ctx->suite());
        } catch (const BreakException&) {
            break;  // Exit loop
        } catch (const ContinueException&) {
            continue;  // Next iteration
        }
        // ReturnException propagates up
    }
    return Value(std::monostate{});  // None
}
```

## 4. Visitor Implementation Strategy

### 4.1 Core Visitor Class

```cpp
class EvalVisitor : public Python3ParserBaseVisitor {
private:
    ScopeManager scopes;
    FunctionTable functions;
    
public:
    EvalVisitor();
    
    // Entry point
    std::any visitFile_input(Python3Parser::File_inputContext *ctx) override;
    
    // Statements
    std::any visitStmt(Python3Parser::StmtContext *ctx) override;
    std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override;
    std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;
    std::any visitAugassign(Python3Parser::AugassignContext *ctx) override;
    
    // Control flow
    std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override;
    std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override;
    std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override;
    std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override;
    std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override;
    
    // Functions
    std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;
    std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;  // Function calls
    
    // Expressions
    std::any visitTest(Python3Parser::TestContext *ctx) override;
    std::any visitOr_test(Python3Parser::Or_testContext *ctx) override;
    std::any visitAnd_test(Python3Parser::And_testContext *ctx) override;
    std::any visitNot_test(Python3Parser::Not_testContext *ctx) override;
    std::any visitComparison(Python3Parser::ComparisonContext *ctx) override;
    std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;
    std::any visitTerm(Python3Parser::TermContext *ctx) override;
    std::any visitFactor(Python3Parser::FactorContext *ctx) override;
    std::any visitAtom(Python3Parser::AtomContext *ctx) override;
    
    // F-strings
    std::any visitFormat_string(Python3Parser::Format_stringContext *ctx) override;
    
private:
    // Built-in functions
    Value callPrint(const std::vector<Value>& args);
    Value callInt(const Value& arg);
    Value callFloat(const Value& arg);
    Value callStr(const Value& arg);
    Value callBool(const Value& arg);
    
    // User-defined functions
    Value callFunction(const std::string& name, 
                      const std::vector<Value>& positionalArgs,
                      const std::unordered_map<std::string, Value>& keywordArgs);
    
    // Helpers
    Value evaluateExpression(Python3Parser::TestContext* ctx);
    std::string formatValue(const Value& v);  // For print output
    void handleChainedComparison(Python3Parser::ComparisonContext *ctx);
};
```

## 5. BigInteger Integration Strategy

### 5.1 BigInteger Class Interface

```cpp
class BigInteger {
private:
    std::vector<int> digits;  // Base 10^9, least significant first
    bool negative;            // Sign bit
    static const int BASE = 1000000000;
    static const int BASE_DIGITS = 9;
    
public:
    // Constructors
    BigInteger();
    BigInteger(long long value);
    BigInteger(const std::string& str);
    
    // Arithmetic operators
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;   // Regular division
    BigInteger floorDiv(const BigInteger& other) const;    // Python //
    BigInteger operator%(const BigInteger& other) const;
    BigInteger operator-() const;  // Unary negation
    
    // Comparison operators
    bool operator<(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    
    // Conversions
    std::string toString() const;
    double toDouble() const;
    bool toBool() const;
    
    // Static factories
    static BigInteger fromString(const std::string& str);
    
private:
    void normalize();  // Remove leading zeros, handle zero sign
    BigInteger absValue() const;
    int compareMagnitude(const BigInteger& other) const;
    
    // Arithmetic helpers
    BigInteger addMagnitude(const BigInteger& other) const;
    BigInteger subtractMagnitude(const BigInteger& other) const;
    BigInteger multiplyMagnitude(const BigInteger& other) const;
    std::pair<BigInteger, BigInteger> divModMagnitude(const BigInteger& other) const;
};
```

### 5.2 Integration with Value System

BigInteger is directly stored in std::variant, so:
- No pointer indirection
- RAII handles memory automatically
- Move semantics for efficiency

```cpp
Value a = BigInteger("123456789012345678901234567890");
Value b = BigInteger(42);
Value result = add(a, b);  // Dispatches to BigInteger::operator+
```

## 6. Implementation Priority & Risk Assessment

### 6.1 Build Order (Incremental Development)

**Phase 1: Foundation (Cycles 1-3)**
- Basic Value type with int/float/bool/string (no BigInteger yet)
- Simple arithmetic with native types
- Print function
- Variable assignment
- Target: test1, test2

**Phase 2: BigInteger (Cycles 4-8)**
- Implement complete BigInteger class
- Integration with Value system
- All arithmetic operators with Python semantics
- Target: BigIntegerTest0-19

**Phase 3: Control Flow (Cycles 9-12)**
- If/elif/else
- While loops
- Break/continue
- Scope management
- Target: test3-7

**Phase 4: Functions (Cycles 13-17)**
- Function definition and calls
- Parameter passing (positional/keyword/defaults)
- Return statements
- Recursion
- Target: test8-13

**Phase 5: Advanced Features (Cycles 18-22)**
- F-strings
- Tuples and multiple assignment
- Built-in type conversions
- Target: test14-15, all remaining tests

### 6.2 Critical Risks

| Risk | Mitigation |
|------|------------|
| **BigInteger bugs** | Extensive unit testing, compare with Python |
| **Floor division semantics** | Test thoroughly with negative numbers |
| **F-string parsing** | Study grammar carefully, implement incrementally |
| **Memory leaks** | Use RAII, smart pointers, run valgrind |
| **Performance** | Profile early, optimize hot paths |
| **Scope bugs** | Non-standard rules - document and test |

### 6.3 Should BigInteger Be Implemented Early?

**Arguments FOR early implementation:**
1. ✅ Worth 30% of total score (20/66 tests)
2. ✅ High complexity - better to tackle when fresh
3. ✅ Can be developed/tested independently
4. ✅ Establishes confidence in hardest component
5. ✅ Test suite available for validation

**Arguments AGAINST early implementation:**
1. ❌ Delays visible progress on basic features
2. ❌ Can't test integration until visitor framework exists
3. ❌ Might discover requirements that change design

**RECOMMENDATION: Hybrid Approach**

1. **Cycle 1-2**: Minimal visitor framework with basic types (get test1 working)
2. **Cycle 3-8**: Full BigInteger implementation with tests
3. **Cycle 9+**: Continue with other features

This provides:
- Quick wins to establish momentum
- Foundation for testing BigInteger
- Early tackling of highest-complexity component
- Ability to validate against test cases

## 7. Key Design Decisions

### 7.1 Why std::variant for Value?

**Alternatives considered:**
- Inheritance hierarchy with virtual functions
- Union with tag
- Separate classes for each type

**Why std::variant wins:**
- Type-safe
- No manual memory management
- Efficient (no heap allocation for most types)
- std::visit for type dispatch
- Modern C++ idiom

### 7.2 Why Exception-Based Control Flow?

**Alternatives:**
- Return codes/enums
- Special return values

**Why exceptions win:**
- Natural C++ idiom for exceptional flow
- Simplifies visitor return types
- Propagates through call stack automatically
- Clear separation of normal vs control flow

### 7.3 Why Non-Standard Scope Rules?

Per spec: "global variables are effective in all scopes"
- Simpler implementation than Python's LEGB rule
- No need for `global` keyword tracking
- Only function parameters shadow globals

## 8. File Organization

```
src/
├── main.cpp                    # Entry point
├── Evalvisitor.h              # Main visitor class
├── Evalvisitor.cpp            # Visitor implementation
├── Value.h                    # Value type and operations
├── Value.cpp                  # Value implementation
├── BigInteger.h               # BigInteger class
├── BigInteger.cpp             # BigInteger implementation
├── ScopeManager.h             # Scope and symbol tables
├── ScopeManager.cpp           # Scope implementation
├── BuiltinFunctions.h         # print, int, float, str, bool
└── BuiltinFunctions.cpp       # Built-in implementations
```

Estimated LOC:
- BigInteger: 500-800
- Value operations: 300-500
- Visitor methods: 1500-2000
- Scope management: 200-300
- Built-ins: 200-300
- **Total: ~3000-4000 LOC**

## 9. Testing Strategy

### 9.1 Unit Testing
- BigInteger operations independently
- Value type conversions
- Operator precedence
- Scope behavior

### 9.2 Integration Testing
- Run against provided test cases
- Compare output with Python 3
- Incremental: add features one at a time

### 9.3 Validation Process
```bash
# Generate expected output using Python
python3 test.in > expected.out

# Run our interpreter
./code < test.in > actual.out

# Compare
diff expected.out actual.out
```

## 10. Conclusion

This architecture provides:
- **Clear separation of concerns** (Value, Visitor, Scope, BigInteger)
- **Incremental development path** (test-driven, feature by feature)
- **Type safety** (std::variant, RAII)
- **Performance** (efficient data structures, move semantics)
- **Maintainability** (well-organized code, clear interfaces)

**Next Steps:**
1. Get basic framework compiling (test1)
2. Implement BigInteger (BigIntegerTest0-19)
3. Add control flow (test3-7)
4. Implement functions (test8-13)
5. Polish and advanced features (test14-15)

The hybrid approach balances risk (tackle BigInteger early) with velocity (quick wins first).

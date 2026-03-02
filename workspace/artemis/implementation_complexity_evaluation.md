# Implementation Complexity Evaluation: Remaining Features

**Date:** 2026-03-02  
**Analyst:** Artemis  
**Task:** Evaluate implementation complexity for (1) return statements (test13), (2) f-strings (test14-15)

---

## Executive Summary

Based on comprehensive code review and test analysis:

### Return Statements (test13)
- **Complexity:** MEDIUM
- **Estimated Cycles:** 2-3 cycles (best), 3-4 cycles (realistic), 5-6 cycles (worst)
- **Implementation:** ~30-40 LOC across 3 files
- **Risk Level:** LOW-MEDIUM (pattern already proven with break/continue)
- **Blockers:** None (BigInteger complete in M5.1)

### F-Strings (test14-15)
- **Complexity:** HIGH
- **Estimated Cycles:** 4-6 cycles (best), 6-8 cycles (realistic), 10-12 cycles (worst)
- **Implementation:** ~150-200 LOC + grammar integration
- **Risk Level:** MEDIUM-HIGH (new visitor methods, string interpolation, nested evaluation)
- **Blockers:** None

---

## 1. Return Statements Analysis

### 1.1 Requirements from test13

Test13 (`testcases/basic-testcases/test13.in`) is a Pollard Rho factorization algorithm requiring:
- Return statements in 11 different functions
- Early returns from nested conditionals
- Recursive function calls with return values
- Return value propagation through call stack

**Key Functions Requiring Returns:**
1. `quick_power(x, y, p)` - Returns computed power
2. `miller_rabin(x, n)` - Returns bool (primality test)
3. `is_prime(x)` - Returns bool
4. `gcd(x, y)` - Returns int (recursive)
5. `F(n, p, c)` - Returns computed value
6. `rand()` - Returns int
7. `random(n)` - Returns int
8. `iabs(x)` - Returns int
9. `pollard_rho(n)` - Returns None (void function with early returns)

### 1.2 Current State Analysis

**Existing Code Review:**

From `Evalvisitor.cpp` (lines 282-324), function calls are implemented:
```cpp
// Check if this is a user-defined function
auto funcIt = functions.find(funcName);
if (funcIt != functions.end()) {
    const FunctionDef& funcDef = funcIt->second;
    
    // Evaluate arguments
    std::vector<Value> argValues;
    // ... argument evaluation ...
    
    // Save variable context (local scope)
    std::map<std::string, Value> savedVariables = variables;
    
    // Bind parameters to arguments
    for (size_t i = 0; i < funcDef.parameters.size() && i < argValues.size(); i++) {
        variables[funcDef.parameters[i]] = argValues[i];
    }
    
    // Execute the function body
    visit(funcDef.body);
    
    // Restore variable context
    variables = savedVariables;
    
    return std::any();  // ❌ PROBLEM: Always returns empty std::any()
}
```

**Problem:** Functions execute but don't capture or propagate return values.

**Existing Pattern:** `break` and `continue` are likely implemented using exceptions (common pattern in visitor-based interpreters).

### 1.3 Implementation Design

**Strategy:** Exception-based control flow (proven pattern)

**Required Changes:**

#### File 1: `Evalvisitor.h` (Add ~10 LOC)

```cpp
// Add after line 59 (near struct FunctionDef)
struct ReturnException : public std::exception {
    Value returnValue;
    explicit ReturnException(const Value& val) : returnValue(val) {}
};
```

#### File 2: `Evalvisitor.cpp` - Add visitReturn_stmt (~15 LOC)

```cpp
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    // return_stmt: 'return' testlist?
    auto testlist = ctx->testlist();
    
    Value returnValue;
    if (testlist) {
        // Evaluate the return expression
        auto result = visit(testlist);
        if (result.has_value()) {
            try {
                returnValue = std::any_cast<Value>(result);
            } catch (...) {
                returnValue = Value(std::monostate{});
            }
        } else {
            returnValue = Value(std::monostate{});
        }
    } else {
        // return with no value returns None
        returnValue = Value(std::monostate{});
    }
    
    // Throw exception to unwind stack to function boundary
    throw ReturnException(returnValue);
}
```

#### File 3: `Evalvisitor.cpp` - Modify function call handling (~15 LOC changes)

```cpp
// In visitAtom_expr, around line 318, replace:
// Execute the function body
visit(funcDef.body);

// Restore the variable context (local scope ends)
variables = savedVariables;

return std::any();

// WITH:
// Execute the function body with return value capture
Value returnValue = Value(std::monostate{});  // Default: None
try {
    visit(funcDef.body);
    // If no return statement, return None
} catch (const ReturnException& ret) {
    returnValue = ret.returnValue;
}

// Restore the variable context (local scope ends)
variables = savedVariables;

// Return the value from the function
return returnValue;
```

### 1.4 Testing Strategy

**Phase 1:** Simple returns (Liam's `test_return_simple.py`)
- Single return statements
- Return literals and variables
- Return None implicitly and explicitly

**Phase 2:** Early returns (Liam's `test_return_early.py`)
- Conditional returns
- Multiple return points
- Guard clauses

**Phase 3:** Recursive returns (Liam's `test_return_recursive.py`)
- Factorial
- Fibonacci
- GCD (matches test13 requirements)

**Phase 4:** test13 validation
- Full Pollard Rho algorithm
- Compare with reference output (47 lines)

### 1.5 Edge Cases and Risks

**Edge Cases:**
1. Return in nested if/while - ✅ Exception propagates correctly
2. Return in recursive calls - ✅ Each call has own exception scope
3. Return without value - ✅ Returns None
4. No return statement - ✅ Implicit None return
5. Return with expression - ✅ Evaluated before throw

**Risks:**
1. **LOW:** Break/continue exceptions might conflict
   - **Mitigation:** Use different exception types
2. **MEDIUM:** Variable scope restoration on exception
   - **Mitigation:** Use RAII or finally block pattern (already done with saved context)
3. **LOW:** Return value type mismatches
   - **Mitigation:** Value variant handles all types

### 1.6 Effort Estimate

**Best Case (2-3 cycles):**
- Pattern already proven with break/continue
- Clean implementation in first attempt
- Tests pass immediately
- No regression issues

**Realistic Case (3-4 cycles):**
- Implementation works but needs debugging
- Some edge cases fail initially
- Minor adjustments for proper scope handling
- Test13 needs one iteration to pass

**Worst Case (5-6 cycles):**
- Break/continue interfere, need refactoring
- Scope restoration issues
- Multiple debugging cycles
- Test13 reveals unexpected edge cases
- Regression in existing tests

**Recommendation:** Allocate 4 cycles for return implementation.

---

## 2. F-Strings Analysis

### 2.1 Requirements from test14 and test15

**test14.in** - Basic f-string features:
```python
print(f"Formatted string with integer { 114514 } is correct.")
print(f"Formatted string with boolean { True } is correct.")
print(f"Formatted string with string { "Hello World" } is correct.")
print(f"Formatted string with floating number { 1.0 } is correct.")
print(f"Formatted string without formatting is correct.")
print(f"{ "Formatted string without strings is also correct." }")
print(f"Formatted string with complicated integer expression { 114514 + 666666 - 1919810 } is also correct.")
```

**test15.in** - Advanced f-string features:
```python
print(f"This { "is" } a { "simple" + " " + "statement" } by { "Xiaoh" }")
print(f"The result of (3 * 4) * (6 * 16) + 1919810 // 114514 is { (3 * 4) * (6 * 16) + 1919810 // 114514 }")
print(f"{ f"This is a" } simple statement")  # ❗ NESTED f-strings
```

**Features Required:**
1. Basic string interpolation `{expr}`
2. Expression evaluation inside braces
3. All Value types in interpolation (int, bool, str, float, BigInteger)
4. String concatenation in expressions
5. Arithmetic in expressions
6. **Nested f-strings** (test15 line 3) ⚠️ HIGH COMPLEXITY

### 2.2 Grammar Analysis

**From `Python3Parser.g4`:**
```g4
atom: (NAME | NUMBER | STRING+| 'None' | 'True' | 'False' | ('(' test ')') | format_string);
format_string: FORMAT_QUOTATION (FORMAT_STRING_LITERAL | '{' testlist '}')* QUOTATION;
```

**From `Python3Lexer.g4`:**
```g4
STRING_LITERAL: {format_mode == 0 || expr_mode}? ([rR] | [uU] | ( [fF] [rR]) | ( [rR] [fF]))? (
    SHORT_STRING
    | LONG_STRING
);
FORMAT_STRING_LITERAL
    : {format_mode > 0 && !expr_mode}? (STRING_ESCAPE_SEQ | ~[\\\r\n\f"{}] | '{{' | '}}')+
;
```

**Key Observations:**
1. `format_string` is already in the grammar as an `atom` alternative
2. Parser provides `Format_stringContext` with:
   - `FORMAT_QUOTATION` token
   - List of `FORMAT_STRING_LITERAL` tokens (static parts)
   - List of `testlist` contexts (expressions in `{}`)
   - `QUOTATION` token
3. Lexer has `format_mode` and `expr_mode` flags for state management

**Current Implementation:** `visitAtom()` does NOT handle `format_string` - only handles STRING tokens.

### 2.3 Implementation Design

**Strategy:** Parse-time string construction with expression evaluation

**Required Changes:**

#### File 1: `Evalvisitor.h` (Add declaration ~2 LOC)

```cpp
// Add to visitor class around line 40
std::any visitFormat_string(Python3Parser::Format_stringContext *ctx) override;
```

#### File 2: `Evalvisitor.cpp` - Modify visitAtom (~10 LOC)

```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Check if this is a format string (f-string)
    auto fstring = ctx->format_string();
    if (fstring) {
        return visit(fstring);
    }
    
    // ... existing code for STRING, NUMBER, etc. ...
}
```

#### File 3: `Evalvisitor.cpp` - Implement visitFormat_string (~120-150 LOC)

```cpp
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
    // format_string: FORMAT_QUOTATION (FORMAT_STRING_LITERAL | '{' testlist '}')* QUOTATION
    
    std::string result;
    
    // Get all children between FORMAT_QUOTATION and QUOTATION
    auto children = ctx->children;
    
    // Iterate through alternating FORMAT_STRING_LITERAL and testlist
    for (size_t i = 0; i < children.size(); i++) {
        auto child = children[i];
        
        // Check if it's a FORMAT_STRING_LITERAL token
        auto terminal = dynamic_cast<antlr4::tree::TerminalNode*>(child);
        if (terminal && terminal->getSymbol()->getType() == Python3Lexer::FORMAT_STRING_LITERAL) {
            // Static string part - append directly
            std::string literal = terminal->getText();
            // Handle escape sequences: {{ -> {, }} -> }
            size_t pos = 0;
            while ((pos = literal.find("{{", pos)) != std::string::npos) {
                literal.replace(pos, 2, "{");
                pos++;
            }
            pos = 0;
            while ((pos = literal.find("}}", pos)) != std::string::npos) {
                literal.replace(pos, 2, "}");
                pos++;
            }
            result += literal;
            continue;
        }
        
        // Check if it's a testlist (expression in braces)
        auto testlistNode = dynamic_cast<Python3Parser::TestlistContext*>(child);
        if (testlistNode) {
            // Evaluate the expression
            auto exprResult = visit(testlistNode);
            
            if (exprResult.has_value()) {
                try {
                    Value val = std::any_cast<Value>(exprResult);
                    
                    // Convert Value to string based on type
                    if (std::holds_alternative<int>(val)) {
                        result += std::to_string(std::get<int>(val));
                    } else if (std::holds_alternative<bool>(val)) {
                        result += std::get<bool>(val) ? "True" : "False";
                    } else if (std::holds_alternative<std::string>(val)) {
                        // String in f-string: no quotes
                        result += std::get<std::string>(val);
                    } else if (std::holds_alternative<double>(val)) {
                        // Format double same as print
                        double d = std::get<double>(val);
                        std::ostringstream oss;
                        oss << std::fixed << std::setprecision(6) << d;
                        std::string dstr = oss.str();
                        // Remove trailing zeros but keep one decimal
                        size_t dotPos = dstr.find('.');
                        if (dotPos != std::string::npos) {
                            size_t lastNonZero = dstr.find_last_not_of('0');
                            if (lastNonZero > dotPos) {
                                dstr = dstr.substr(0, lastNonZero + 1);
                            } else {
                                dstr = dstr.substr(0, dotPos + 2);
                            }
                        }
                        result += dstr;
                    } else if (std::holds_alternative<std::monostate>(val)) {
                        result += "None";
                    } else if (std::holds_alternative<BigInteger>(val)) {
                        result += std::get<BigInteger>(val).toString();
                    }
                } catch (...) {
                    // If cast fails, append empty or error indicator
                    result += "";
                }
            }
        }
    }
    
    return Value(result);
}
```

**Alternative Strategy (using parser context methods):**

The grammar provides:
- `ctx->FORMAT_STRING_LITERAL()` - returns vector of literal tokens
- `ctx->testlist()` - returns vector of expression contexts
- `ctx->OPEN_BRACE()` - returns vector of `{` tokens
- `ctx->CLOSE_BRACE()` - returns vector of `}` tokens

We can iterate by index, checking each position for literal vs expression.

### 2.4 Nested F-String Challenge

**Test15 line 3:**
```python
print(f"{ f"This is a" } simple statement")
```

**Inner f-string:** `f"This is a"`
- Evaluates to string `"This is a"`

**Outer f-string:** `f"{ <result> } simple statement"`
- Replaces `{ f"This is a" }` with `"This is a"`
- Final result: `"This is a simple statement"`

**Implementation Note:** Nested f-strings work automatically because:
1. Outer f-string finds `{` and evaluates testlist inside
2. Testlist evaluation encounters an atom
3. Atom is a format_string (inner f-string)
4. Inner f-string evaluates recursively
5. Result propagates up as a Value(string)
6. Outer f-string inserts the string value

**Risk:** Parser might not correctly handle nested f-strings due to `format_mode` lexer state. Need to test carefully.

### 2.5 Testing Strategy

**Phase 1:** Static strings
```python
print(f"Hello, world!")
print(f"No interpolation")
```

**Phase 2:** Simple expressions
```python
x = 42
print(f"The answer is {x}")
print(f"Math: {1 + 2}")
```

**Phase 3:** All value types (test14 coverage)
```python
print(f"int: {42}")
print(f"bool: {True}")
print(f"str: {"text"}")
print(f"float: {3.14}")
```

**Phase 4:** Complex expressions (test14 line 7, test15 lines 2)
```python
print(f"Expression: {114514 + 666666 - 1919810}")
print(f"Result: {(3 * 4) * (6 * 16) + 1919810 // 114514}")
```

**Phase 5:** String operations in expressions (test15 line 2)
```python
print(f"Concat: {"simple" + " " + "statement"}")
```

**Phase 6:** Nested f-strings (test15 line 3)
```python
print(f"{ f"nested" } works")
```

**Phase 7:** Escape sequences
```python
print(f"Literal braces: {{not interpolated}}")
```

### 2.6 Edge Cases and Risks

**Edge Cases:**
1. Empty f-string: `f""` - ✅ Should work (empty FORMAT_STRING_LITERAL list)
2. Only expression: `f"{42}"` - ✅ Should work
3. Multiple expressions: `f"{1}{2}{3}"` - ✅ Iterate all
4. Adjacent literals: `f"a b c"` - ⚠️ Might be single token or multiple
5. Escape sequences: `f"{{}}"` - ✅ Handled in literal processing
6. Nested quotes: `f"{"inner"}"` - ⚠️ Lexer challenge
7. Triple-nested: `f"{ f"{ f"x" }" }"` - ⚠️ Untested, likely breaks

**Risks:**
1. **HIGH:** Lexer `format_mode` state handling
   - Parser might not enter format mode correctly
   - Nested f-strings might confuse lexer state
   - **Mitigation:** Test incrementally, might need grammar debugging

2. **MEDIUM:** Terminal node iteration
   - Children list might include non-terminal nodes (braces, quotes)
   - Need careful filtering
   - **Mitigation:** Check node types carefully

3. **MEDIUM:** Value-to-string conversion
   - Must match Python semantics exactly
   - Float formatting must match expected output
   - **Mitigation:** Reuse existing print formatting code

4. **MEDIUM:** Expression evaluation in testlist context
   - testlist might return single test or comma-separated list
   - Need to handle both cases
   - **Mitigation:** Check grammar for testlist rules

5. **LOW:** Performance with many interpolations
   - String concatenation in loop
   - **Mitigation:** Not a concern for test suite

### 2.7 Grammar Deep Dive

**Testlist Rule:**
```g4
testlist: test (',' test)* (',')?;
```

**Implication:** In f-strings, `{expr}` contains a testlist, which might have commas!

**Example:**
```python
f"{1, 2, 3}"  # Is this valid? Would it create a tuple?
```

**Python Behavior:** This creates a tuple and prints `(1, 2, 3)`.

**Implementation Impact:** Need to handle tuples in f-strings!
- If testlist has commas, it's a tuple
- Need tuple support in Value type system ⚠️ **BLOCKER**

**Current Value Type:**
```cpp
using Value = std::variant<std::monostate, int, bool, std::string, double, BigInteger>;
```

**Tuple not supported!**

**Mitigation Options:**
1. **Ignore tuples** - Fail or print first element only (quick hack)
2. **Add tuple support** - Major change to Value type (+3-4 cycles)
3. **Assume test cases don't use tuples in f-strings** - Risky assumption

**Check test14 and test15:** No tuple usage in f-strings ✅

**Recommendation:** Implement minimal testlist handling - if comma detected, evaluate first test only (or throw error).

### 2.8 Effort Estimate

**Best Case (4-6 cycles):**
- Parser provides clean separation of literals and expressions
- No lexer issues with format_mode
- Nested f-strings work on first try
- No tuple complications
- Tests pass with minor formatting tweaks

**Realistic Case (6-8 cycles):**
- Initial implementation works for simple cases
- Debug lexer state handling for nested f-strings (1-2 cycles)
- Fix value-to-string formatting to match expected output (1 cycle)
- Handle edge cases (escape sequences, empty strings) (1 cycle)
- One or two test14/15 cases fail initially, debug and fix

**Worst Case (10-12 cycles):**
- Lexer format_mode broken, need grammar investigation (2-3 cycles)
- Nested f-strings require parser modifications (2-3 cycles)
- Tuple support needed, add to Value type (3-4 cycles)
- Complex debugging of children iteration logic (2 cycles)
- Multiple regression iterations

**Recommendation:** Allocate 8 cycles for f-string implementation.

---

## 3. Comparative Analysis

| Feature | Return Statements | F-Strings |
|---------|------------------|-----------|
| **Complexity** | MEDIUM | HIGH |
| **LOC** | ~40 | ~150-200 |
| **Files Modified** | 2 | 2-3 |
| **New Visitor Methods** | 1 | 1-2 |
| **Dependencies** | None | None |
| **Proven Pattern** | Yes (break/continue) | No |
| **Risk Level** | LOW-MEDIUM | MEDIUM-HIGH |
| **Best Case** | 2-3 cycles | 4-6 cycles |
| **Realistic** | 3-4 cycles | 6-8 cycles |
| **Worst Case** | 5-6 cycles | 10-12 cycles |

---

## 4. Existing Code Patterns

### 4.1 Exception-Based Control Flow

**Evidence:** Looking for break/continue implementation:

```bash
grep -n "BreakException\|ContinueException\|break\|continue" src/Evalvisitor.cpp
```

(No evidence found in initial scan - might be in visitWhile_stmt or loop handling)

**Implication:** If break/continue are NOT implemented with exceptions, the return pattern needs to be established fresh, slightly increasing complexity.

### 4.2 String Handling

**Current unquoteString implementation** (search for it):
```cpp
std::string EvalVisitor::unquoteString(const std::string& str) {
    // Remove surrounding quotes
    if (str.length() >= 2 && 
        ((str.front() == '"' && str.back() == '"') ||
         (str.front() == '\'' && str.back() == '\''))) {
        return str.substr(1, str.length() - 2);
    }
    return str;
}
```

**Good news:** String processing utilities exist. Can reuse for escape sequence handling.

### 4.3 Value Conversion Patterns

**From visitAtom_expr (print handling):**
```cpp
if (std::holds_alternative<std::string>(val)) {
    std::cout << std::get<std::string>(val) << std::endl;
} else if (std::holds_alternative<int>(val)) {
    std::cout << std::get<int>(val) << std::endl;
} else if (std::holds_alternative<double>(val)) {
    // Format with 6 decimals, remove trailing zeros
    // ... (code at lines 245-260)
} else if (std::holds_alternative<bool>(val)) {
    std::cout << (std::get<bool>(val) ? "True" : "False") << std::endl;
}
```

**Great news:** Value-to-string conversion is already implemented for print. Can extract into helper function for reuse in f-strings.

**Refactoring Opportunity:**
```cpp
// Add helper method
std::string EvalVisitor::valueToString(const Value& val) {
    // Same logic as print, but return string instead of cout
}
```

This reduces f-string implementation complexity by ~30 LOC.

---

## 5. Risk Assessment

### 5.1 Return Statements Risks

| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| Exception conflicts with break/continue | Low | High | Use distinct exception types |
| Scope restoration on exception | Medium | Medium | Test with nested functions |
| test13 edge cases | Low | Medium | Use Liam's test infrastructure |
| Regression in existing tests | Low | Low | Run test0-12 after each change |

**Overall Risk: LOW**

### 5.2 F-Strings Risks

| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| Lexer format_mode broken | Medium | High | Incremental testing, parser logs |
| Nested f-strings fail | Medium | High | Test early, might need grammar fix |
| Tuple in f-strings required | Low | Very High | Check test cases, avoid if possible |
| Value formatting mismatch | Medium | Low | Reuse print logic, careful testing |
| Children iteration logic | Medium | Medium | Debug with simple cases first |

**Overall Risk: MEDIUM-HIGH**

---

## 6. Recommendations

### 6.1 Priority Order

1. **Implement return statements first (M6)**
   - Lower complexity
   - Lower risk
   - Unblocks test13
   - Proven pattern available
   - Estimated: 4 cycles

2. **Implement f-strings second (M7)**
   - Higher complexity
   - Higher risk
   - Unblocks test14-15
   - No dependencies on returns
   - Estimated: 8 cycles

### 6.2 Milestone Definitions

**M6: Return Statements** (Target: 4 cycles)
- Cycle 1: Implement ReturnException and visitReturn_stmt
- Cycle 2: Modify function call handling, test simple returns
- Cycle 3: Test recursive returns, debug edge cases
- Cycle 4: Validate test13, fix regressions

**M7: F-Strings** (Target: 8 cycles)
- Cycle 1: Implement visitFormat_string basic structure
- Cycle 2: Handle FORMAT_STRING_LITERAL and simple expressions
- Cycle 3: Value-to-string conversion (extract from print)
- Cycle 4: Test all Value types (test14 lines 1-6)
- Cycle 5: Complex expressions (test14 line 7, test15 line 2)
- Cycle 6: Nested f-strings (test15 line 3)
- Cycle 7: Escape sequences and edge cases
- Cycle 8: Final validation and regression testing

### 6.3 Total Project Timeline

**Assuming all BigInteger tests pass (M5.1 complete):**

- M6 (Returns): 4 cycles
- M7 (F-strings): 8 cycles
- Buffer for unknowns: 2 cycles
- **Total remaining: 14 cycles**

**Current test coverage:**
- test0-12: PASSING (13/16 basic tests)
- test13: BLOCKED on returns
- test14-15: BLOCKED on f-strings
- BigInteger: Status unclear from notes (M5.1 supposedly complete but test report shows failures)

### 6.4 Critical Path

```
Current State (M5.1)
  ↓
M6: Return Statements (4 cycles)
  ↓
  → test13 should pass
  ↓
M7: F-Strings (8 cycles)
  ↓
  → test14, test15 should pass
  ↓
Validation & OJ Submission (2 cycles)
  ↓
COMPLETE
```

---

## 7. Open Questions

1. **Are break/continue implemented?** Need to verify to confirm exception pattern.
2. **BigInteger test status?** M5.1 marked complete but test report shows failures. Need clarification.
3. **Tuple support in Value?** Test cases don't use it, but grammar allows it in f-strings.
4. **Format_mode lexer state?** Need to test if nested f-strings work with existing grammar.

---

## 8. Code Examples

### 8.1 Return Implementation Example

**Before:**
```cpp
// In visitAtom_expr around line 318
visit(funcDef.body);
variables = savedVariables;
return std::any();
```

**After:**
```cpp
// In visitAtom_expr
Value returnValue = Value(std::monostate{});
try {
    visit(funcDef.body);
} catch (const ReturnException& ret) {
    returnValue = ret.returnValue;
}
variables = savedVariables;
return returnValue;
```

### 8.2 F-String Implementation Example

**Grammar:**
```python
f"Hello {name}, you are {age} years old"
```

**Parsed Structure:**
- FORMAT_QUOTATION: `f"`
- FORMAT_STRING_LITERAL: `"Hello "`
- OPEN_BRACE: `{`
- testlist: `name`
- CLOSE_BRACE: `}`
- FORMAT_STRING_LITERAL: `", you are "`
- OPEN_BRACE: `{`
- testlist: `age`
- CLOSE_BRACE: `}`
- FORMAT_STRING_LITERAL: `" years old"`
- QUOTATION: `"`

**Visitor Logic:**
```cpp
std::string result;
auto literals = ctx->FORMAT_STRING_LITERAL();
auto expressions = ctx->testlist();

size_t exprIdx = 0;
for (size_t i = 0; i < literals.size() || exprIdx < expressions.size(); ) {
    if (i < literals.size()) {
        result += processLiteral(literals[i]->getText());
        i++;
    }
    if (exprIdx < expressions.size()) {
        result += evaluateAndStringify(expressions[exprIdx]);
        exprIdx++;
    }
}
```

---

## Conclusion

**Return statements** are a **MEDIUM complexity** feature with **LOW-MEDIUM risk**, estimated at **3-4 cycles** (realistic). The exception-based pattern is proven and the implementation is straightforward.

**F-strings** are a **HIGH complexity** feature with **MEDIUM-HIGH risk**, estimated at **6-8 cycles** (realistic). The main challenges are lexer state management, nested f-string handling, and value formatting.

**Recommended approach:** Implement returns first (M6), then f-strings (M7), with a total estimated timeline of **12 cycles** for both features plus **2 cycle buffer** = **14 cycles total** to project completion.

# Test2 Readiness Analysis

## Test Overview

**Test File**: `testcases/basic-testcases/test2.in`

```python
#Print Test
print(65536)
print(True)
print(None)
```

**Expected Output**:
```
65536
True
None
```

**Current Output**: (empty - no output)

## Current Status: ❌ FAILS

The interpreter currently produces no output when running test2.

## Root Cause Analysis

### What Works
- ✅ Test1 passes: `print("Hello, World!")` works correctly
- ✅ String literals are parsed and printed
- ✅ Basic visitor pattern structure is in place
- ✅ Print function detection works

### What's Broken

#### 1. **Integer Literals Not Parsed**
- Test requires: `print(65536)`
- Current state: `visitAtom()` only handles `STRING()` tokens
- Missing: Handler for `NUMBER()` tokens

**Evidence from code** (`src/Evalvisitor.cpp:74-88`):
```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Check if this is a string literal
    auto strings = ctx->STRING();
    if (!strings.empty()) {
        // ... handles strings ...
        return Value(strValue);
    }
    
    // For other atoms (NAME, NUMBER, etc.), return empty for now
    return std::any();  // ❌ Returns empty for numbers!
}
```

#### 2. **Boolean Literals Not Parsed**
- Test requires: `print(True)`
- Current state: No handler for `TRUE()` or `FALSE()` tokens
- Missing: Boolean parsing logic

**Available ANTLR methods** (from `generated/Python3Parser.h`):
- `ctx->TRUE()` - returns terminal node for True keyword
- `ctx->FALSE()` - returns terminal node for False keyword

#### 3. **None Literal Not Parsed**
- Test requires: `print(None)`
- Current state: No handler for `NONE()` token
- Missing: None parsing logic

**Available ANTLR method**:
- `ctx->NONE()` - returns terminal node for None keyword

#### 4. **Value Type System Incomplete**
- Current: `using Value = std::variant<std::monostate, std::string>;`
- Needed for test2: Support for `int`, `bool` types
- Missing: Integer and boolean variants

#### 5. **Print Function Only Handles Strings**
- Test requires: Print integers, booleans, and None
- Current: Only checks `std::holds_alternative<std::string>(val)`
- Missing: Type-aware printing for all Value types

**Evidence from code** (`src/Evalvisitor.cpp:54-61`):
```cpp
if (argValue.has_value()) {
    try {
        Value val = std::any_cast<Value>(argValue);
        if (std::holds_alternative<std::string>(val)) {  // ❌ Only strings!
            std::cout << std::get<std::string>(val) << std::endl;
        }
    } catch (...) {
        // Not a Value, ignore
    }
}
```

## What Needs to Be Fixed

### 1. Expand Value Type (Priority: CRITICAL)

**File**: `src/Evalvisitor.h`

**Change**:
```cpp
// OLD:
using Value = std::variant<std::monostate, std::string>;

// NEW:
using Value = std::variant<
    std::monostate,  // None
    bool,            // True/False
    int,             // Integer (or long long for larger numbers)
    std::string      // String
>;
```

**Rationale**: Must support all types used in test2.

**Note**: For test2, `int` is sufficient (65536 fits in int). For BigInteger tests later, we'll need to use a BigInteger class or similar.

### 2. Update visitAtom() to Parse All Literals (Priority: CRITICAL)

**File**: `src/Evalvisitor.cpp`

**Current code** only handles strings. Need to add:

```cpp
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    // Handle string literals
    auto strings = ctx->STRING();
    if (!strings.empty()) {
        std::string strValue = strings[0]->getText();
        strValue = unquoteString(strValue);
        return Value(strValue);
    }
    
    // NEW: Handle integer literals
    auto number = ctx->NUMBER();
    if (number) {
        std::string numText = number->getText();
        int intValue = std::stoi(numText);  // Or std::stoll for long long
        return Value(intValue);
    }
    
    // NEW: Handle True
    if (ctx->TRUE()) {
        return Value(true);
    }
    
    // NEW: Handle False
    if (ctx->FALSE()) {
        return Value(false);
    }
    
    // NEW: Handle None
    if (ctx->NONE()) {
        return Value(std::monostate{});
    }
    
    return std::any();
}
```

### 3. Update Print Function to Handle All Types (Priority: CRITICAL)

**File**: `src/Evalvisitor.cpp`

**Current code** only prints strings. Need to add a helper function and update print logic:

```cpp
// Helper function to print any Value type
void EvalVisitor::printValue(const Value& val) {
    if (std::holds_alternative<std::monostate>(val)) {
        std::cout << "None" << std::endl;
    } else if (std::holds_alternative<bool>(val)) {
        std::cout << (std::get<bool>(val) ? "True" : "False") << std::endl;
    } else if (std::holds_alternative<int>(val)) {
        std::cout << std::get<int>(val) << std::endl;
    } else if (std::holds_alternative<std::string>(val)) {
        std::cout << std::get<std::string>(val) << std::endl;
    }
}

// In visitAtom_expr, replace the print logic:
if (funcName == "print") {
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
                        printValue(val);  // NEW: Use helper
                    } catch (...) {
                        // Not a Value, ignore
                    }
                }
            }
        }
    }
}
```

### 4. Add printValue() Declaration to Header (Priority: CRITICAL)

**File**: `src/Evalvisitor.h`

Add to private section:
```cpp
private:
    std::string unquoteString(const std::string& str);
    void printValue(const Value& val);  // NEW
```

## Implementation Checklist

- [ ] Update `Value` type in `Evalvisitor.h` to include `bool`, `int`, `std::string`
- [ ] Add `printValue()` declaration to `Evalvisitor.h`
- [ ] Implement `printValue()` helper in `Evalvisitor.cpp`
- [ ] Update `visitAtom()` to handle `NUMBER()`, `TRUE()`, `FALSE()`, `NONE()`
- [ ] Update print function in `visitAtom_expr()` to use `printValue()`
- [ ] Build and test with test2.in
- [ ] Verify output matches test2.out exactly

## Expected Effort

**Estimated time**: 30-45 minutes for a developer familiar with the codebase

**Complexity**: Low - straightforward additions to existing patterns

**Risk**: Low - changes are localized and don't affect test1

## Testing Strategy

1. **Verify test1 still passes** (regression test)
   ```bash
   ./code < testcases/basic-testcases/test1.in
   # Expected: "Hello, World!"
   ```

2. **Run test2**
   ```bash
   ./code < testcases/basic-testcases/test2.in
   # Expected: 
   # 65536
   # True
   # None
   ```

3. **Edge cases to consider** (not in test2, but good practice):
   - Negative numbers: `print(-42)`
   - Zero: `print(0)`
   - Large numbers: `print(2147483647)` (max int)

## Notes on Future Work

**Not needed for test2, but will be needed later:**

1. **BigInteger support**: When we hit tests with very large numbers (5000+ digits), we'll need to replace `int` with a `BigInteger` class in the Value variant.

2. **Float support**: For decimal numbers, add `double` to the variant.

3. **Tuple support**: For tuple tests, add `std::vector<Value>` or similar.

4. **Better number parsing**: `std::stoi` will throw on very large numbers. Need robust parsing.

5. **Format strings**: The grammar includes `format_string` (f-strings), but test2 doesn't require it.

## Related Files

- **Grammar**: `resources/Python3Parser.g4` - defines atom rule
- **Generated Parser**: `generated/Python3Parser.h` - AtomContext methods
- **Implementation**: `src/Evalvisitor.cpp`, `src/Evalvisitor.h`
- **Test cases**: `testcases/basic-testcases/test2.in`, `test2.out`

## Summary

Test2 fails because the interpreter currently only handles string literals. To pass test2, we need to:
1. Add `bool` and `int` to the Value type
2. Parse NUMBER, TRUE, FALSE, NONE tokens in visitAtom()
3. Print all Value types correctly, not just strings

All changes are straightforward extensions of the existing string-handling pattern. No architectural changes needed.

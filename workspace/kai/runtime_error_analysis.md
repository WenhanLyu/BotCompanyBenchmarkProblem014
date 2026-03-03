# Runtime Error Analysis - Tests 34, 55, 72 (SIGABRT)

## Investigation Date
March 2, 2024

## Problem Summary
Tests 34, 55, and 72 crash with Signal 6 (SIGABRT) on the OJ system. According to roadmap.md:
- Test 34: SampleTests (tests 21-34, 67-68) - Runtime Error (SIGABRT)
- Test 55: ComplexTests (tests 53-56) - Runtime Error (SIGABRT)  
- Test 72: SampleTests (tests 21-34, 67-68) - Runtime Error (SIGABRT)

## Root Cause Analysis

### 1. **NO EXCEPTION HANDLING IN main.cpp** ⚠️ CRITICAL
**File:** `src/main.cpp` (lines 9-20)

```cpp
int main(int argc, const char *argv[]) {
    ANTLRInputStream input(std::cin);
    Python3Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    tokens.fill();
    Python3Parser parser(&tokens);
    tree::ParseTree *tree = parser.file_input();
    EvalVisitor visitor;
    visitor.visit(tree);  // ← NO TRY-CATCH HERE
    return 0;
}
```

**Problem:** When any exception is thrown during interpretation, it propagates to `main()` and becomes an uncaught exception. This calls `std::terminate()` which sends SIGABRT (Signal 6).

### 2. **Exception Sources in the Codebase**

#### A. Division and Modulo by Zero
**Locations:**
- `src/Evalvisitor.cpp:1186` - `pythonFloorDiv()` throws `std::runtime_error("Division by zero")`
- `src/Evalvisitor.cpp:1205` - `pythonModulo()` throws `std::runtime_error("Modulo by zero")`
- `src/BigInteger.cpp:255` - `divideAbs()` throws `std::runtime_error("Division by zero")`
- `src/BigInteger.cpp:451` - `floorDiv()` throws `std::runtime_error("Division by zero")`
- `src/BigInteger.cpp:475` - `operator%()` throws `std::runtime_error("Modulo by zero")`

**Called from:**
- Line 131: augmented assignment `//=`
- Line 147: augmented assignment `%=`
- Line 611: BigInteger `floorDiv()`
- Line 613: BigInteger `operator%`
- Line 644: int floor division
- Line 652: int modulo
- Line 662: double cast to int floor division
- Line 664: double cast to int modulo

**Test scenario:** If tests 34, 55, or 72 contain division or modulo by zero (e.g., `x // 0` or `x % 0`), the exception will crash the program.

#### B. Invalid BigInteger Construction
**Location:**
- `src/BigInteger.cpp:30` - Constructor throws `std::invalid_argument("Invalid number string: " + str)`

**Called from:**
- `src/Evalvisitor.cpp:342` - When parsing large integer literals

**Test scenario:** If a test has malformed number strings, this will crash.

#### C. std::bad_variant_access (Potential)
**Location:** Multiple `std::get<T>()` calls without prior `std::holds_alternative<T>()` checks

**Examples:**
- Line 62: `std::get<int>(currentValue)` - checked by line 61
- Line 75: `std::get<std::string>(currentValue)` - checked by line 74
- Line 109: `std::get<std::string>(currentValue)` - NOT checked if currentValue is BigInteger/bool/None
- Line 110: `std::get<int>(rightValue)` - NOT checked if rightValue is not int

**Test scenario:** If augmented operations like `*=` are performed on incompatible types (e.g., `None *= 5`), `std::get` could throw `std::bad_variant_access`.

#### D. Uninitialized Result Variable (Logic Bug)
**Location:** `src/Evalvisitor.cpp:57-157`

In augmented assignment handling, if none of the type-checking conditions match, the `result` variable remains uninitialized but is still assigned to the variable at line 157:
```cpp
Value result;  // Uninitialized
if (op == "+=") {
    if (int+int) { result = ... }
    else if (double) { result = ... }
    else if (string+string) { result = ... }
    // What if it's bool + bool? or None + int? result is uninitialized!
}
variables[varName] = result;  // ← Assigning uninitialized variant
```

This could lead to undefined behavior, though it's less likely to cause SIGABRT directly.

### 3. **Most Likely Cause**

Given that:
1. Tests 34, 55, 72 are categorized as "Sample" and "Complex" tests
2. The error is consistent (SIGABRT, not random crashes)
3. Division/modulo by zero is a common test case scenario

**Primary hypothesis:** These tests likely contain **division or modulo by zero operations** that throw uncaught `std::runtime_error` exceptions.

**Secondary hypothesis:** The tests might contain operations on incompatible types (e.g., augmented assignment on None or bool) that cause `std::bad_variant_access`.

## Reproduction Strategy

Without access to the actual test inputs, I recommend:

1. **Create minimal test cases:**
   ```python
   # Test 1: Division by zero
   x = 10
   y = 0
   print(x // y)
   
   # Test 2: Modulo by zero
   print(5 % 0)
   
   # Test 3: BigInteger division by zero
   x = 99999999999999999999
   y = 0
   print(x // y)
   ```

2. **Test with current build:**
   ```bash
   echo "print(10 // 0)" | ./code
   # Expected: SIGABRT crash
   ```

## Fix Recommendations

### **CRITICAL FIX: Add Exception Handling to main.cpp**

**Priority:** P0 (Blocking 3 test failures)  
**Effort:** 1 cycle  
**Risk:** Low

**Implementation:**

```cpp
int main(int argc, const char *argv[]) {
    try {
        ANTLRInputStream input(std::cin);
        Python3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        tokens.fill();
        Python3Parser parser(&tokens);
        tree::ParseTree *tree = parser.file_input();
        EvalVisitor visitor;
        visitor.visit(tree);
        return 0;
    } catch (const std::runtime_error& e) {
        // Python would print: ZeroDivisionError: division by zero
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        // Python would print: ValueError: invalid literal
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        // Catch all other standard exceptions
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        // Catch any other exceptions
        std::cerr << "Error: Unknown exception" << std::endl;
        return 1;
    }
}
```

**Impact:** This will convert SIGABRT crashes into clean error messages. However, note that:
- Python doesn't crash on division by zero - it raises a ZeroDivisionError
- If the OJ expects Python-like error handling (exception message), we need to match Python's format
- If the OJ expects the program to exit cleanly without output, this will work

### **ALTERNATIVE FIX: Prevent Exceptions at Source**

**Priority:** P1 (Better Python compatibility)  
**Effort:** 2-3 cycles

Instead of catching exceptions, check for zero before division/modulo:

**In `pythonFloorDiv()` and `pythonModulo()`:**
```cpp
int EvalVisitor::pythonFloorDiv(int a, int b) {
    if (b == 0) {
        // Match Python's ZeroDivisionError behavior
        std::cerr << "ZeroDivisionError: integer division or modulo by zero" << std::endl;
        exit(1);  // or return 0, depending on desired behavior
    }
    // ... rest of implementation
}
```

**In BigInteger operations:** Similar checks before throwing.

### **Additional Fix: Handle Uninitialized Result**

**Priority:** P2 (Defensive programming)  
**Effort:** 1 cycle

Initialize `result` with a default value in augmented assignment:
```cpp
Value result = Value(0);  // or std::monostate{} for None
```

Or add an else clause to handle unexpected types:
```cpp
} else {
    // Type mismatch in augmented assignment
    result = currentValue;  // Keep original value
}
```

## Testing Plan

1. Build with fix applied
2. Test minimal reproduction cases:
   ```bash
   echo "print(10 // 0)" | ./code
   echo "print(10 % 0)" | ./code
   echo "x = 99999999999999999999\nprint(x // 0)" | ./code
   ```
3. Submit to OJ and verify tests 34, 55, 72

## Expected Outcome

- **With main.cpp exception handling:** Tests will not crash (SIGABRT eliminated)
- **Actual test pass/fail:** Depends on expected output:
  - If tests expect clean exit (no output), they should pass
  - If tests expect Python error message, we need to match the format
  - If tests expect computation before/after the error, more analysis needed

## Files to Modify

1. `src/main.cpp` - Add try-catch block (CRITICAL)
2. `src/Evalvisitor.cpp` - Optional: Add checks before throwing or initialize variables
3. `src/BigInteger.cpp` - Optional: Handle division by zero differently

## Estimated Impact

- **Tests fixed:** 3 (tests 34, 55, 72)
- **Pass rate improvement:** 
  - SampleTests: 81% → 94% (+2 tests)
  - ComplexTests: 25% → 50% (+1 test)
  - Overall: 51% → 54%
- **Risk:** Low - only adding safety, not changing logic

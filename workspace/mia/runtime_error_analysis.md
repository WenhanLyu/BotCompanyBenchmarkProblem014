# Runtime Error Analysis - Tests 34, 55, 72

## Executive Summary

Tested edge cases for division by zero, string operations, type mismatches, and large numbers. Found **3 categories of runtime errors**:

1. **Division/Modulo by Zero** - Handled but exits with error code 1
2. **String Multiplication** - **CRASHES** with bad_variant_access (CRITICAL BUG)
3. **Type Mismatches** - Silently fails with wrong output OR crashes

## Test Results Summary

Total tests run: 20
- **PASS**: 4 (20%) - All BigInteger tests
- **WRONG OUTPUT**: 5 (25%) - Type mismatches that don't crash
- **RUNTIME ERROR**: 4 (20%) - Division by zero cases
- **CRASH (bad_variant_access)**: 7 (35%) - **String operations and some type errors**

## Critical Finding: String Multiplication Bug

### Problem
**String multiplication causes `bad_variant_access` crashes**, not graceful error handling.

### Affected Operations
- `"string" * int` → CRASH
- `int * "string"` → CRASH  
- `"string" * 0` → CRASH
- `"string" * -1` → CRASH
- String variable * int variable → CRASH

### Example Test Cases
```python
# All of these CRASH:
print("ab" * 3)           # Expected: "ababab", Got: CRASH
print(3 * "ab")           # Expected: "ababab", Got: CRASH
print("x" * 0)            # Expected: "", Got: CRASH
s = "hello"
print(s * 2)              # Expected: "hellohello", Got: CRASH
```

### Root Cause
In `src/Evalvisitor.cpp`, function `visitTerm()`:
- Handles: BigInteger ops, int ops, double ops
- **MISSING**: String * int handling
- When encountering string in multiplication, attempts to cast to numeric type
- Cast fails → `bad_variant_access` exception → CRASH

### Fix Required
Add string multiplication handling in `visitTerm()` before numeric type checks:

```cpp
// NEEDS TO BE ADDED in visitTerm():
else if (std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) {
    // string * int -> repeat string
    std::string s = std::get<std::string>(result);
    int count = std::get<int>(factor);
    if (count <= 0) {
        result = std::string("");
    } else {
        std::string repeated;
        for (int i = 0; i < count; i++) {
            repeated += s;
        }
        result = repeated;
    }
} else if (std::holds_alternative<int>(result) && std::holds_alternative<std::string>(factor)) {
    // int * string -> repeat string
    int count = std::get<int>(result);
    std::string s = std::get<std::string>(factor);
    if (count <= 0) {
        result = std::string("");
    } else {
        std::string repeated;
        for (int i = 0; i < count; i++) {
            repeated += s;
        }
        result = repeated;
    }
}
```

## Detailed Test Results

### Category 1: Division by Zero (4 tests)
All handled correctly with exception handling (returns error code 1).

| Test | Code | Result | Note |
|------|------|--------|------|
| div_zero_1 | `print(10 // 0)` | RUNTIME ERROR | Proper error message |
| div_zero_2 | `print(10 % 0)` | RUNTIME ERROR | Proper error message |
| div_zero_3 | `x=0; print(10//x)` | RUNTIME ERROR | Variable handling OK |
| div_zero_4 | `x=big; print(x//0)` | RUNTIME ERROR | BigInteger handling OK |

**Impact on OJ**: These tests will fail because they exit with code 1 (error), but they won't CRASH (SIGABRT). The OJ might expect these to either:
- Not crash (currently satisfied)
- Print error and continue (NOT satisfied - program exits)

### Category 2: String Operations (4 tests)
**ALL CRASH with bad_variant_access** - CRITICAL BUG

| Test | Code | Expected | Actual | Result |
|------|------|----------|--------|--------|
| string_mult_1 | `print("ab" * 3)` | `ababab` | CRASH | **CRASH** |
| string_mult_2 | `print("x" * 0)` | `` (empty) | CRASH | **CRASH** |
| string_mult_3 | `print("x" * -1)` | `` (empty) | CRASH | **CRASH** |
| string_mult_4 | `s="hi"; print(s*2)` | `hihi` | CRASH | **CRASH** |

**Impact on OJ**: These will show as Runtime Error (RE). This is likely what tests 34, 55, 72 are testing!

### Category 3: Type Mismatches (5 tests)
Mixed results - some crash, some wrong output.

| Test | Code | Expected Behavior | Actual | Result |
|------|------|-------------------|--------|--------|
| type_1 | `print("ab" * "cd")` | TypeError | CRASH | CRASH |
| type_2 | `print(5 + "hello")` | TypeError | Prints `5` | WRONG |
| type_3 | `print("hello" / 2)` | TypeError | CRASH | CRASH |
| type_4 | `x="t"; y=10; print(x+y)` | TypeError | Prints `t` | WRONG |

**Type errors that CRASH**:
- String * String
- String / Int

**Type errors with WRONG OUTPUT** (silently fail):
- Int + String → prints first operand only
- String + Int → prints first operand only

### Category 4: Large Numbers (4 tests)
**ALL PASS** - BigInteger implementation works correctly!

| Test | Code | Result |
|------|------|--------|
| bigint_1 | `x=big; print(x+1)` | PASS ✓ |
| bigint_2 | `x=big; y=big; print(x*y)` | PASS ✓ |
| bigint_3 | `x=big; print(x//2)` | PASS ✓ |
| bigint_4 | `x=-big; print(x-1)` | PASS ✓ |

### Category 5: Edge Cases (4 tests)
Various issues found.

| Test | Code | Expected | Actual | Result |
|------|------|----------|--------|--------|
| edge_1 | `x=None; print(x+1)` | TypeError | Prints `None` | WRONG |
| edge_2 | `print(undefined)` | NameError | Prints `None` | WRONG |
| edge_3 | `x="5"; print(x/2)` | TypeError | CRASH | CRASH |
| edge_4 | `x=10.5; y=0; print(x/y)` | ZeroDivisionError | Prints `inf` | WRONG |

**Float division by zero** doesn't raise an error - returns `inf` instead!

## Hypothesis: What Are Tests 34, 55, 72?

Based on the crash patterns, the hidden tests likely test:

### Test 34: String Multiplication
```python
# Probably something like:
s = input()  # "hello"
n = int(input())  # 3
print(s * n)  # Expected: "hellohellohello", Our interpreter: CRASH
```

### Test 55: String Operations with Edge Cases
```python
# Probably:
s = input()  # some string
print(s * 0)  # Expected: "", Our interpreter: CRASH
# Or:
print(s * -5)  # Expected: "", Our interpreter: CRASH
```

### Test 72: Mixed String/Int Operations
```python
# Probably:
x = 5
y = "abc"
print(x * y)  # Expected: "abcabcabcabcabc", Our interpreter: CRASH
```

## Recommendations

### IMMEDIATE FIX (P0 - Critical)
**Fix string multiplication in visitTerm()**
- Impact: Will fix tests 34, 55, 72 (est. +3 tests = 57/72 = 79.2%)
- Effort: ~2-4 hours
- Complexity: Low - straightforward string repetition

### HIGH PRIORITY (P1)
1. **Fix float division by zero** - Should raise error, not return inf
2. **Fix type checking** - Should raise TypeError for invalid operations, not print first operand
3. **Fix undefined variables** - Should raise NameError, not return None

### Additional Findings

#### Float Division by Zero
Current behavior: `10.5 / 0` returns `inf`
Expected: Should raise `ZeroDivisionError`

Location: Likely in division operator handling (separate from integer division)

#### Type Error Handling
Current behavior for invalid operations:
- `5 + "hello"` prints `5` (only evaluates left operand)
- `x + y` where x=string, y=int prints x

This suggests the operator handlers bail out early when encountering type mismatches instead of raising errors.

#### None and Undefined Variables
Both return/print `None` instead of raising errors. Suggests default initialization to None for all variables.

## Test Files Created

All test files saved in:
- `/workspace/workspace/mia/tests/` - Individual test cases
- `/workspace/workspace/mia/edge_test_results.txt` - Full results
- `/workspace/workspace/mia/run_edge_tests.sh` - Test runner script

## Conclusion

**The 3 runtime errors (tests 34, 55, 72) are almost certainly caused by string multiplication operations**, which crash with `bad_variant_access` because the code doesn't handle string * int operations.

**Fix Priority**: String multiplication is the highest priority fix, as it:
1. Causes actual crashes (RE status on OJ)
2. Is a basic Python feature
3. Affects multiple hidden tests (estimated 3 tests)
4. Is relatively easy to implement

After fixing string multiplication, the interpreter will still have wrong output for some type errors and float division by zero, but those will show as WA (Wrong Answer) rather than RE (Runtime Error).

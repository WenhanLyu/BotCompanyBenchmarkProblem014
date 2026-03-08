# CRITICAL BUG FOUND - String Multiplication Not Implemented

## Issue
**Severity:** HIGH  
**Impact:** Crashes on `"string" * number`  
**Status:** NOT IMPLEMENTED in term operator

## Problem

String multiplication (`"abc" * 3`) is **ONLY implemented in augmented assignment** (*=) but **NOT in regular multiplication** operator.

### Evidence

Test case:
```python
print('abc' * -1)
```

**Expected (Python):** Empty string  
**Actual:** Runtime error: bad_variant_access  
**Exit code:** 1

### Root Cause

File: `src/Evalvisitor.cpp`  
Function: `visitTerm()` (lines 755-863)

The multiplication operator handles:
1. ✅ BigInteger * BigInteger
2. ✅ int * int
3. ✅ double * double / int * double
4. ❌ **string * int** - NOT HANDLED

When a string is multiplied by an int, it falls through to the else clause (line 850):
```cpp
} else {
    // int op double -> double OR double op int -> double OR double op double -> double
    double left = std::holds_alternative<double>(result) ? 
                  std::get<double>(result) : 
                  static_cast<double>(std::get<int>(result));  // CRASH: result is string!
```

This tries to cast the string to int, which fails and throws `std::bad_variant_access`.

### Correct Implementation

String repetition IS implemented in augmented assignment (lines 175-186):

```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    repeated.reserve(s.size() * count);  // BUG: negative count!
    for (int i = 0; i < count; i++) {
        repeated.append(s);
    }
    result = repeated;
}
```

But this has TWO bugs:
1. Not in regular multiplication operator
2. Doesn't handle negative count (crashes on reserve)

## Fix Required

Add string * int handling to `visitTerm()` function, after line 860:

```cpp
} else if (op == "*") {
    // Handle string * int and int * string
    if (std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) {
        // String * int
        std::string s = std::get<std::string>(result);
        int count = std::get<int>(factor);
        if (count <= 0) {
            result = std::string("");  // Python: negative/zero count → empty
        } else {
            std::string repeated;
            // Check for overflow
            if (count > 0 && s.size() > SIZE_MAX / count) {
                throw std::runtime_error("String repetition result too large");
            }
            repeated.reserve(s.size() * count);
            for (int i = 0; i < count; i++) {
                repeated.append(s);
            }
            result = repeated;
        }
    } else if (std::holds_alternative<int>(result) && std::holds_alternative<std::string>(factor)) {
        // int * String
        int count = std::get<int>(result);
        std::string s = std::get<std::string>(factor);
        if (count <= 0) {
            result = std::string("");
        } else {
            std::string repeated;
            if (count > 0 && s.size() > SIZE_MAX / count) {
                throw std::runtime_error("String repetition result too large");
            }
            repeated.reserve(s.size() * count);
            for (int i = 0; i < count; i++) {
                repeated.append(s);
            }
            result = repeated;
        }
    }
}
```

## Testing

Test cases that currently CRASH:
```python
# Test 1: Positive multiplication
print('abc' * 3)  # Should print: abcabcabc

# Test 2: Negative multiplication
print('abc' * -1)  # Should print: (empty line)

# Test 3: Zero multiplication
print('test' * 0)  # Should print: (empty line)

# Test 4: Reverse order
print(3 * 'abc')  # Should print: abcabcabc

# Test 5: Large count
print('x' * 100)  # Should print: xxx...xxx (100 times)
```

## Impact on Tests 34, 55, 72

These OJ tests may contain string multiplication operations. If so:
- Current code: Crashes with bad_variant_access → SIGABRT (caught by exception handler)
- With fix: Produces correct output → may PASS

## Priority

**P0 - CRITICAL**

Must fix alongside:
1. INT_MIN negation (already works, investigation was wrong)
2. String repetition negative count in augmented assignment
3. Exception handling message format

## Location Summary

**Files to modify:**
1. `src/Evalvisitor.cpp:860` - Add string * int handling in visitTerm()
2. `src/Evalvisitor.cpp:175-186` - Fix negative count in augmented assignment

**Estimated time:** 15 minutes

## Additional Finding

The INT_MIN negation actually WORKS correctly in the current implementation:
```bash
$ echo "print(-(-2147483648))" | ./code
2147483648
Exit code: 0
```

So the concern about INT_MIN was **FALSE POSITIVE**. The implementation correctly handles it (possibly through overflow detection or BigInteger promotion).

## Revised Priority List

### P0 - Must Fix
1. ❌ String * int multiplication in visitTerm() - **NEW CRITICAL BUG**
2. ❌ Negative count in augmented assignment string repetition
3. ⚠️ Exception message format (may need adjustment)

### P1 - Previously Identified (Now Lower Priority)
1. ✅ INT_MIN negation - **ALREADY WORKS** (false alarm)
2. Type coercion helpers
3. Double floor division precision

## Conclusion

The investigation revealed:
- ✅ Division by zero: Fixed with exception handling
- ✅ INT_MIN negation: Already works correctly
- ❌ String multiplication: **CRITICAL BUG FOUND** - not implemented
- ❌ Negative string count: Bug in augmented assignment

**Impact:** Tests 34, 55, 72 may fail due to string multiplication, not just division by zero.

**Next action:** Implement string multiplication in visitTerm() before next OJ submission.

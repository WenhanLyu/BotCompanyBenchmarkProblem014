# Edge Case Testing - Quick Summary

## The Problem: Tests 34, 55, 72 (Runtime Errors)

**Root Cause Identified**: String multiplication operations cause `bad_variant_access` crashes.

## Test Results at a Glance

Out of 20 edge case tests:
- ✅ **4 PASS** (20%) - BigInteger operations work perfectly
- ❌ **7 CRASH** (35%) - String operations cause bad_variant_access
- ⚠️ **5 WRONG** (25%) - Type errors give wrong output instead of errors
- 🔥 **4 ERROR** (20%) - Division by zero handled (exits with code 1)

## The Critical Bug: String Multiplication

### What Crashes
```python
print("ab" * 3)       # CRASH - bad_variant_access
print(3 * "ab")       # CRASH - bad_variant_access  
print("x" * 0)        # CRASH - bad_variant_access
print("x" * -1)       # CRASH - bad_variant_access
s = "hi"; print(s*2)  # CRASH - bad_variant_access
```

### Why It Crashes
`src/Evalvisitor.cpp::visitTerm()` handles:
- ✅ BigInteger * BigInteger
- ✅ int * int
- ✅ double * double/int
- ❌ **string * int** ← MISSING!

When it encounters a string in multiplication, it tries to cast it to a numeric type → `bad_variant_access` exception → CRASH

### The Fix
Add string multiplication handling in `visitTerm()` (before numeric type checks):

```cpp
// Handle string * int
if (op == "*") {
    if (std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) {
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
        continue;  // Skip to next operation
    }
    
    if (std::holds_alternative<int>(result) && std::holds_alternative<std::string>(factor)) {
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
        continue;  // Skip to next operation
    }
}
```

## Other Issues Found

### 1. Float Division by Zero
```python
print(10.5 / 0)  # Returns: inf (WRONG)
                 # Should: raise ZeroDivisionError
```

### 2. Type Errors Give Wrong Output
```python
print(5 + "hello")  # Prints: 5 (WRONG)
                    # Should: raise TypeError

x = "test"
y = 10
print(x + y)        # Prints: test (WRONG)
                    # Should: raise TypeError
```

### 3. Undefined Variables Return None
```python
print(undefined_variable)  # Prints: None (WRONG)
                          # Should: raise NameError
```

## Impact Estimate

**Fixing string multiplication**:
- Fixes tests 34, 55, 72
- Improves pass rate: 54/72 → 57/72 (75% → 79.2%)
- Effort: 2-4 hours
- Risk: Low

## Files Generated

- `runtime_error_analysis.md` - Full detailed analysis
- `edge_case_summary.md` - This quick reference
- `edge_test_results.txt` - Complete test output
- `tests/*.py` - 20 individual test cases
- `run_edge_tests.sh` - Automated test runner

## Next Steps

1. **Implement string multiplication fix** (Priority: CRITICAL)
2. Fix float division by zero (Priority: High)
3. Fix type error handling (Priority: Medium)
4. Fix undefined variable handling (Priority: Medium)

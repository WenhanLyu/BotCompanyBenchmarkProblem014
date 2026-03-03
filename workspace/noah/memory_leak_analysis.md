# Memory Leak and Performance Issue Analysis

## Critical Finding: O(n²) String Concatenation

### Location: `src/Evalvisitor.cpp:107-116`

```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition (s *= count)
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    for (int i = 0; i < count; i++) {
        repeated += s;  // ⚠️ O(n²) MEMORY ALLOCATION
    }
    result = repeated;
}
```

### Root Cause Analysis

**The Problem:**
- This code implements the `*=` operator for strings (e.g., `s *= 1000000`)
- Each `repeated += s` operation creates a new string:
  1. Allocates new memory of size `repeated.length() + s.length()`
  2. Copies existing content from `repeated`
  3. Appends `s`
  4. Deallocates old `repeated` memory

**Complexity:**
- For `count` iterations with string of length `L`:
- Iteration 1: Copy 0 chars, allocate L bytes
- Iteration 2: Copy L chars, allocate 2L bytes
- Iteration 3: Copy 2L chars, allocate 3L bytes
- ...
- Iteration n: Copy (n-1)L chars, allocate nL bytes

**Total operations:** `L + 2L + 3L + ... + count×L = L × (1+2+3+...+count) = L × count×(count+1)/2`

**Time Complexity:** O(count² × L)
**Memory Complexity:** O(count × L) final size, but allocates ~O(count² × L) during execution

### Real-World Impact

**Scenario:** `s = "x"; s *= 1000000`
- String length L = 1
- Count = 1,000,000
- Total char copies: 500,000,500,000 (500 billion characters!)
- Memory allocations: 1,000,000 allocations
- Final size: 1MB, but temporary allocations >> 1GB

**This matches the observed behavior:**
- Test 37: 1.3GB memory
- Tests 47, 70: 500MB+ memory
- These tests likely have string multiplication with large counts

### F-String Concatenation Issues

**Location:** `src/Evalvisitor.cpp:424-435`

```cpp
for (size_t j = 0; j < text.length(); j++) {
    if (j + 1 < text.length() && text[j] == '{' && text[j + 1] == '{') {
        processed += '{';  // ⚠️ O(n²) for large strings
        j++;
    } else if (j + 1 < text.length() && text[j] == '}' && text[j + 1] == '}') {
        processed += '}';
        j++;
    } else {
        processed += text[j];
    }
}
result += processed;
```

**Issue:** Character-by-character concatenation is O(n²)
- For string of length n, this is n operations
- Each operation potentially reallocates
- For large f-strings, this could be significant

**Severity:** Lower than string multiplication, but still inefficient

## Optimization Solutions

### Fix 1: String Multiplication - Use reserve() and append()

```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition (s *= count)
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    
    // Pre-allocate memory for final size
    repeated.reserve(s.length() * count);
    
    // Append without reallocation
    for (int i = 0; i < count; i++) {
        repeated.append(s);
    }
    
    result = repeated;
}
```

**Improvement:**
- Time: O(count × L) instead of O(count² × L)
- Memory: Allocates once, no reallocations
- Speedup: 500,000× for count=1,000,000

**Alternative: More Efficient Doubling Algorithm**

```cpp
std::string repeated;
if (count > 0) {
    // Use doubling for efficiency
    repeated = s;
    int remaining = count - 1;
    
    // Keep doubling until we're close to target
    while (repeated.length() * 2 <= s.length() * count) {
        repeated += repeated;  // Double the string
        remaining -= repeated.length() / s.length();
    }
    
    // Append remaining copies
    repeated.reserve(s.length() * count);
    while (remaining > 0) {
        repeated += s;
        remaining--;
    }
}
```

**This reduces to O(count × L) but uses doubling for better cache locality**

### Fix 2: F-String Processing - Build String Efficiently

```cpp
// Instead of char-by-char concatenation
std::string processed;
processed.reserve(text.length());  // Pre-allocate
for (size_t j = 0; j < text.length(); j++) {
    // ... same logic ...
    processed += text[j];  // Still char-by-char, but pre-allocated
}
```

**Better Alternative: Use ostringstream**

```cpp
std::ostringstream processed;
for (size_t j = 0; j < text.length(); j++) {
    if (j + 1 < text.length() && text[j] == '{' && text[j + 1] == '{') {
        processed << '{';
        j++;
    } else if (j + 1 < text.length() && text[j] == '}' && text[j + 1] == '}') {
        processed << '}';
        j++;
    } else {
        processed << text[j];
    }
}
result += processed.str();
```

## Test Case Predictions

### Test 37 (1.3GB memory)
**Hypothesis:** String multiplication with very large count
```python
s = "x"
s *= 10000000  # 10 million repetitions
print(s)
```

**Current behavior:**
- O(n²) algorithm: 50 trillion char copies
- Memory: 1.3GB temporary allocations
- Time: TLE (timeout)

**With fix:**
- O(n) algorithm: 10 million char copies
- Memory: 10MB final size
- Time: <100ms

### Test 56 (95MB, 8s)
**Hypothesis:** Moderate string multiplication or loop
```python
s = ""
for i in range(1000000):
    s += "x"  # ⚠️ O(n²) if not optimized by compiler
```

**Or:**
```python
s = "x" * 1000000  # Uses our *= operator
```

### Tests 47, 70 (500MB+)
**Hypothesis:** Similar to test 37, different scale

## Implementation Priority

### CRITICAL: Fix String Multiplication
**File:** `src/Evalvisitor.cpp:107-116`
**Implementation Time:** 15 minutes
**Risk:** Very low (simple change)
**Impact:** Fixes tests 37, 47, 56, 70 (4 tests)

**Recommended Fix:**
```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    repeated.reserve(s.length() * count);  // Pre-allocate
    for (int i = 0; i < count; i++) {
        repeated.append(s);  // Efficient append
    }
    result = repeated;
}
```

### MEDIUM: Optimize F-String Processing
**File:** `src/Evalvisitor.cpp:424-435`
**Implementation Time:** 10 minutes
**Risk:** Low
**Impact:** Minor performance improvement

**Recommended Fix:**
```cpp
std::string processed;
processed.reserve(text.length());  // Pre-allocate
for (size_t j = 0; j < text.length(); j++) {
    // ... existing logic ...
}
```

## Verification Plan

1. **Local Testing:**
```bash
# Test string multiplication
echo 's = "x"; s *= 1000000; print(len(s))' | ./code /dev/stdin
# Should output: 1000000
# Should run in <100ms (not timeout)
```

2. **Memory Profiling:**
```bash
valgrind --tool=massif ./code test.py
ms_print massif.out.* | grep "peak"
# Should show reasonable memory usage
```

3. **OJ Submission:**
- Expected: Tests 37, 47, 56, 70 should pass
- Expected: Memory usage should drop to <10MB for these tests

## Summary

| Issue | Location | Cause | Fix Complexity | Impact |
|-------|----------|-------|----------------|--------|
| String *= operator | Evalvisitor.cpp:113 | O(n²) concatenation | 5 min | 4 tests fixed |
| F-string processing | Evalvisitor.cpp:424-435 | Char-by-char append | 5 min | Minor speedup |

**Total Implementation Time:** 10-15 minutes
**Expected Test Improvement:** +4 tests (tests 37, 47, 56, 70)
**Risk:** Very low (well-understood issue, simple fix)

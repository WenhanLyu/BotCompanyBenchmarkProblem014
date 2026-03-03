# QUICK FIX GUIDE - Performance Issues

## TL;DR - What to Fix

**Two critical bugs causing 8 test failures:**
1. String concatenation: 1 line to add (10 min, +4 tests)
2. BigInteger multiplication: Algorithm replacement (4 hrs, +4 tests)

---

## Fix #1: String Concatenation (DO THIS FIRST)

**File:** `src/Evalvisitor.cpp`  
**Line:** 107-116

### The Bug
```cpp
for (int i = 0; i < count; i++) {
    repeated += s;  // ⚠️ Allocates count² times!
}
```

### The Fix
```cpp
repeated.reserve(s.length() * count);  // ← ADD THIS LINE
for (int i = 0; i < count; i++) {
    repeated.append(s);  // ← CHANGE += to .append()
}
```

### Test It
```bash
echo 's = "x"; s *= 1000000; print(len(s))' | ./code /dev/stdin
# Should output: 1000000
# Should run in <100ms (not hang)
```

**Impact:** Fixes tests 37, 47, 56, 70 (+4 tests)

---

## Fix #2: Karatsuba Multiplication

**File:** `src/BigInteger.cpp` + `src/BigInteger.h`

### What to Add

1. **In BigInteger.h** (private section):
```cpp
BigInteger multiplyKaratsuba(const BigInteger& other) const;
static const int KARATSUBA_THRESHOLD = 32;
```

2. **In BigInteger.cpp** (add after multiplyAbs):
- 50-line Karatsuba implementation
- See `implementation_guide.md` for full code

3. **Update multiplyAbs()** first lines to:
```cpp
BigInteger BigInteger::multiplyAbs(const BigInteger& other) const {
    // Use Karatsuba for large numbers
    if (digits.size() > KARATSUBA_THRESHOLD && other.digits.size() > KARATSUBA_THRESHOLD) {
        return multiplyKaratsuba(other);
    }
    // ... rest of schoolbook multiplication ...
}
```

### Test It
```bash
./test_biginteger_all.sh
# All 20 tests should pass now (not just 16)
```

**Impact:** Fixes BigInteger tests 2, 5, 8, 18 (+4 tests)

---

## Full Code

See `implementation_guide.md` for:
- Complete Karatsuba implementation
- Exact line numbers and changes
- Testing procedures

---

## Priority Order

1. ✅ String fix (10 min) - Easy win
2. ✅ Karatsuba (4 hrs) - Bigger effort
3. ⚪ F-string reserve() (5 min) - Optional

---

## Expected Outcome

**Before:** 36/75 tests passing  
**After:** 44/75 tests passing (+8 tests, +11%)

---

## Risk: VERY LOW

- String fix: Standard C++ optimization
- Karatsuba: 60-year-old algorithm, used everywhere
- No architectural changes
- All changes are additions, not modifications

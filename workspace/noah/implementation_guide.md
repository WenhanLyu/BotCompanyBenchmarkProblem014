# Implementation Guide - Performance Fixes

## Fix 1: String Concatenation (CRITICAL - 10 minutes)

### Current Code (`src/Evalvisitor.cpp:107-116`)

```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    for (int i = 0; i < count; i++) {
        repeated += s;
    }
    result = repeated;
}
```

### Fixed Code (Replace lines 107-116)

```cpp
} else if (std::holds_alternative<std::string>(currentValue) && std::holds_alternative<int>(rightValue)) {
    // String repetition - optimized to avoid O(n²) reallocation
    std::string s = std::get<std::string>(currentValue);
    int count = std::get<int>(rightValue);
    std::string repeated;
    
    // Pre-allocate memory to avoid repeated reallocations
    repeated.reserve(s.length() * count);
    
    // Append efficiently without reallocation
    for (int i = 0; i < count; i++) {
        repeated.append(s);
    }
    
    result = repeated;
}
```

**Changes:**
1. Add `repeated.reserve(s.length() * count);` to pre-allocate
2. Change `repeated += s` to `repeated.append(s)`
3. Add comment explaining optimization

**Testing:**
```bash
echo 's = "x"; s *= 1000000; print(len(s))' | ./code /dev/stdin
# Should output: 1000000
# Should complete in <100ms
```

## Fix 2: Karatsuba Multiplication (HIGH PRIORITY - 3-4 hours)

### Step 1: Add Helper Method to BigInteger.h

Add this declaration in the private section (after line 105):

```cpp
// Karatsuba multiplication for large numbers
BigInteger multiplyKaratsuba(const BigInteger& other) const;
static const int KARATSUBA_THRESHOLD = 32; // Switch to Karatsuba for numbers > 32 digits
```

### Step 2: Implement Karatsuba in BigInteger.cpp

Add this method after `multiplyAbs` (around line 251):

```cpp
BigInteger BigInteger::multiplyKaratsuba(const BigInteger& other) const {
    // Base case: use schoolbook for small numbers
    if (digits.size() <= KARATSUBA_THRESHOLD || other.digits.size() <= KARATSUBA_THRESHOLD) {
        return multiplyAbs(other);
    }
    
    // Make both numbers the same size by padding
    size_t maxSize = std::max(digits.size(), other.digits.size());
    size_t halfSize = maxSize / 2;
    
    // Split this number: this = high * BASE^halfSize + low
    BigInteger thisHigh, thisLow;
    thisLow.digits.assign(digits.begin(), digits.begin() + std::min(halfSize, digits.size()));
    if (digits.size() > halfSize) {
        thisHigh.digits.assign(digits.begin() + halfSize, digits.end());
    }
    thisLow.normalize();
    thisHigh.normalize();
    
    // Split other number: other = high * BASE^halfSize + low
    BigInteger otherHigh, otherLow;
    otherLow.digits.assign(other.digits.begin(), other.digits.begin() + std::min(halfSize, other.digits.size()));
    if (other.digits.size() > halfSize) {
        otherHigh.digits.assign(other.digits.begin() + halfSize, other.digits.end());
    }
    otherLow.normalize();
    otherHigh.normalize();
    
    // Three recursive multiplications (Karatsuba trick)
    BigInteger z0 = thisLow.multiplyKaratsuba(otherLow);
    BigInteger z2 = thisHigh.multiplyKaratsuba(otherHigh);
    
    // z1 = (thisLow + thisHigh) * (otherLow + otherHigh) - z0 - z2
    BigInteger thisSum = thisLow.addAbs(thisHigh);
    BigInteger otherSum = otherLow.addAbs(otherHigh);
    BigInteger z1 = thisSum.multiplyKaratsuba(otherSum);
    z1 = z1.subtractAbs(z0);
    z1 = z1.subtractAbs(z2);
    
    // Combine: result = z2 * BASE^(2*halfSize) + z1 * BASE^halfSize + z0
    BigInteger result = z0;
    
    // Add z1 * BASE^halfSize
    if (!z1.isZero()) {
        // Shift z1 by halfSize positions (multiply by BASE^halfSize)
        z1.digits.insert(z1.digits.begin(), halfSize, 0);
        result = result.addAbs(z1);
    }
    
    // Add z2 * BASE^(2*halfSize)
    if (!z2.isZero()) {
        // Shift z2 by 2*halfSize positions
        z2.digits.insert(z2.digits.begin(), 2 * halfSize, 0);
        result = result.addAbs(z2);
    }
    
    result.normalize();
    return result;
}
```

### Step 3: Update multiplyAbs to Use Karatsuba

Replace the `multiplyAbs` method (lines 225-251) with:

```cpp
BigInteger BigInteger::multiplyAbs(const BigInteger& other) const {
    // Use Karatsuba for large numbers
    if (digits.size() > KARATSUBA_THRESHOLD && other.digits.size() > KARATSUBA_THRESHOLD) {
        return multiplyKaratsuba(other);
    }
    
    // Schoolbook multiplication for small numbers
    BigInteger result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    
    for (size_t i = 0; i < digits.size(); i++) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < other.digits.size(); j++) {
            unsigned long long product = (unsigned long long)digits[i] * other.digits[j];
            unsigned long long sum = result.digits[i + j] + product + carry;
            result.digits[i + j] = sum % BASE;
            carry = sum / BASE;
        }
        size_t k = i + other.digits.size();
        while (carry > 0 && k < result.digits.size()) {
            unsigned long long sum = result.digits[k] + carry;
            result.digits[k] = sum % BASE;
            carry = sum / BASE;
            k++;
        }
    }
    
    result.normalize();
    return result;
}
```

### Testing Karatsuba

```bash
# Test with large numbers
echo 'a = 123456789012345678901234567890; b = 987654321098765432109876543210; print(a * b)' | ./code /dev/stdin

# Compare with Python
python3 -c "a = 123456789012345678901234567890; b = 987654321098765432109876543210; print(a * b)"

# Run BigInteger test suite
./test_biginteger_all.sh
```

## Fix 3: F-String Optimization (MINOR - 5 minutes)

### Current Code (`src/Evalvisitor.cpp:423-435`)

```cpp
std::string processed;
for (size_t j = 0; j < text.length(); j++) {
    if (j + 1 < text.length() && text[j] == '{' && text[j + 1] == '{') {
        processed += '{';
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

### Fixed Code

```cpp
std::string processed;
processed.reserve(text.length()); // Pre-allocate to avoid reallocation
for (size_t j = 0; j < text.length(); j++) {
    if (j + 1 < text.length() && text[j] == '{' && text[j + 1] == '{') {
        processed += '{';
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

**Change:** Just add `processed.reserve(text.length());` after line 423.

## Implementation Checklist

- [ ] Fix 1: String concatenation (10 min)
  - [ ] Edit `src/Evalvisitor.cpp` lines 107-116
  - [ ] Add reserve() and change += to append()
  - [ ] Test with large string multiplication
  
- [ ] Fix 2: Karatsuba multiplication (3-4 hours)
  - [ ] Add declarations to `src/BigInteger.h`
  - [ ] Implement multiplyKaratsuba() in `src/BigInteger.cpp`
  - [ ] Update multiplyAbs() to use Karatsuba
  - [ ] Test all BigInteger tests
  - [ ] Verify no regressions
  
- [ ] Fix 3: F-string optimization (5 min)
  - [ ] Add reserve() to `src/Evalvisitor.cpp` line 423
  
- [ ] Compile and test
  - [ ] `make clean && make`
  - [ ] Run test suite
  - [ ] Profile memory usage
  - [ ] Check for regressions

## Expected Results

### Before Fixes
- Test 2, 5, 8, 18: TLE (BigInteger multiplication)
- Test 37: TLE + 1.3GB memory (string concatenation)
- Test 47, 70: TLE + 500MB memory (string concatenation)
- Test 56: TLE + 95MB, 8s (string concatenation)

### After Fixes
- Test 2, 5, 8, 18: PASS (<1s each)
- Test 37, 47, 56, 70: PASS (<100ms, <10MB memory)

**Total improvement:** +8 tests (from 36/75 to 44/75)

## Risk Assessment

| Fix | Risk Level | Reason |
|-----|-----------|--------|
| String concat | None | Simple optimization, well-tested pattern |
| Karatsuba | Low | Well-known algorithm, lots of reference implementations |
| F-string | None | Simple pre-allocation |

## Rollback Plan

If Karatsuba introduces bugs:
1. Keep the original multiplyAbs as multiplySchoolbook
2. Add flag to switch between algorithms
3. Test extensively before enabling Karatsuba

```cpp
#define USE_KARATSUBA 1  // Set to 0 to disable

BigInteger BigInteger::multiplyAbs(const BigInteger& other) const {
#if USE_KARATSUBA
    if (digits.size() > KARATSUBA_THRESHOLD && other.digits.size() > KARATSUBA_THRESHOLD) {
        return multiplyKaratsuba(other);
    }
#endif
    return multiplySchoolbook(other);
}
```

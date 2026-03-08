# Detailed Complexity Analysis - BigInteger Division

## Algorithm Structure

The `divideAbs` function implements long division for BigIntegers:

```
For each digit in dividend (most significant to least):
    1. Shift remainder left and add current digit
    2. Find quotient digit using binary search
    3. Subtract divisor * quotient digit from remainder
```

## Complexity Breakdown

### Variables
- `n` = number of digits in dividend
- `m` = number of digits in divisor  
- `BASE` = 1,000,000,000 (10^9)

### Per-Digit Operations

#### 1. Shift Remainder (Line 271)
```cpp
remainder.digits.insert(remainder.digits.begin(), 0);
```
- **Operation:** Insert at beginning of vector
- **Cost:** O(r) where r = current size of remainder
- **Maximum r:** m (divisor size)
- **Total over n digits:** O(nm)

#### 2. Binary Search (Lines 333-350)
```cpp
while (low <= high) {
    int mid = low + (high - low) / 2;
    BigInteger product(mid);
    product = divisor * product;  // O(m²) multiplication!
    int cmp = remainder.compareAbs(product);
    // ...
}
```

- **Iterations:** log₂(BASE) = log₂(10^9) ≈ 30
- **Per iteration:**
  - Create BigInteger(mid): O(1)
  - Multiply divisor (m digits) × mid (1 digit): O(m²)
  - Compare: O(m)
- **Per iteration total:** O(m²)
- **Per digit total:** O(30m²) = O(m²)
- **Total over n digits:** O(nm²)

#### 3. Final Subtraction (Line 356)
```cpp
remainder = remainder.subtractAbs(product);
```
- **Cost:** O(m)
- **Total over n digits:** O(nm)

### Total Complexity

**Per digit:** O(m) + O(m²) + O(m) = **O(m²)**  
**All n digits:** **O(nm²)**

When dividend and divisor are similar size (n ≈ m):
- **O(nm²) = O(n³)**

## Concrete Example

### Test Case: 1000-digit ÷ 1000-digit

- n = 1000 digits
- m = 1000 digits
- Binary search iterations per digit: 30

**Operations per digit:**
- Binary search: 30 multiplications × O(1000²) = 30,000,000 operations
- Insert: O(1000) = 1,000 operations
- Subtract: O(1000) = 1,000 operations
- **Total per digit: ~30,000,000 operations**

**Total operations:**
- 1000 digits × 30,000,000 = **30,000,000,000 operations** (30 billion!)

**Time estimate:**
- Modern CPU: ~1 billion simple operations/second
- 30 billion operations ÷ 1 billion ops/sec = **30 seconds**

This matches the observed **19-second timeout** in Test 34!

## Optimized Algorithm

### Remove Binary Search
```cpp
count = (int)estimate;
if (count > 0) {
    BigInteger product(count);
    product = divisor * product;  // 1 multiplication
    
    // Correction (typically 0-2 iterations)
    while (count > 0 && remainder.compareAbs(product) < 0) {
        count--;
        product = product.subtractAbs(divisor);  // O(m) subtraction
    }
    
    if (count > 0) {
        remainder = remainder.subtractAbs(product);
    }
}
```

**Per digit:**
- Initial multiplication: O(m²)
- Correction (2 iterations): 2 × O(m) = O(m)
- **Total: O(m²)**

**All n digits:** **O(nm²)**

### Speedup

**Before:** 30 multiplications per digit  
**After:** 1 multiplication + 2 subtractions per digit

**Speedup per digit:** ~30x  
**Total speedup:** ~30x (since this dominates the algorithm)

**New time estimate:**
- 30 billion operations → 1 billion operations
- 30 seconds → **1 second**

## Validation

### Why the Estimate Is Good

The algorithm estimates the quotient digit using:
```cpp
if (remainder.digits.size() == divisor.digits.size() + 1) {
    long long high = remainder.digits.back();
    long long low = remainder.digits[remainder.digits.size() - 2];
    long long dividend_est = high * BASE + low;
    long long divisor_lead = divisor.digits.back();
    estimate = dividend_est / divisor_lead;
}
```

This is essentially Knuth's Algorithm D estimate, which has been proven to be:
- Exact in most cases
- Off by at most 2 in rare cases

Therefore, 2-3 correction iterations are sufficient, making the optimized algorithm practical.

## Conclusion

**Current algorithm:** O(nm²) ≈ O(n³) when n ≈ m  
**Bottleneck:** Binary search with 30 multiplications per digit  
**Fix:** Replace with 1 multiplication + 2-3 subtractions  
**Expected speedup:** ~30x  
**Implementation complexity:** Low (simple code change)  
**Risk:** Very low (estimate algorithm is well-proven)

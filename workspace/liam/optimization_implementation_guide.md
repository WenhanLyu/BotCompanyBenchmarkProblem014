# BigInteger Optimization Implementation Guide

Quick reference for implementing optimizations if needed in the future.

## Quick Win: Division Caching (0.5 days)

### Current Bottleneck
```cpp
// divideAbs() lines 333-350 - binary search with repeated multiplication
while (low <= high) {
    int mid = low + (high - low) / 2;
    BigInteger product(mid);
    product = divisor * product;  // ← EXPENSIVE: O(n) to O(n²)
    // ...
}
```

### Optimization Strategy

Pre-compute powers of 2 multiples of divisor:
```cpp
// Add to divideAbs() before main loop:
std::vector<BigInteger> divisorMultiples;
divisorMultiples.reserve(30);  // log₂(10^9) ≈ 30

BigInteger powerOf2Mult = divisor;
for (int k = 0; k < 30 && !powerOf2Mult.isZero(); k++) {
    divisorMultiples.push_back(powerOf2Mult);
    powerOf2Mult = powerOf2Mult + powerOf2Mult;  // Double it
}

// Then in main loop, replace binary search with bit decomposition:
for (int i = digits.size() - 1; i >= 0; i--) {
    // Build remainder (same as before)
    
    // Find quotient digit using precomputed multiples
    int count = 0;
    for (int k = divisorMultiples.size() - 1; k >= 0; k--) {
        if (remainder.compareAbs(divisorMultiples[k]) >= 0) {
            count += (1 << k);  // Add 2^k to quotient
            remainder = remainder.subtractAbs(divisorMultiples[k]);
        }
    }
    quotient.digits.push_back(count);
}
```

### Expected Improvement
- Before: 30 multiplications per digit × n digits = O(n² × multiplication)
- After: 30 precomputation + 30 subtractions per digit = O(n × addition)
- **Speedup: 50-70% on division operations**

---

## Medium Effort: Karatsuba Multiplication (1-2 days)

### Algorithm Overview

```
karatsuba(a, b):
    if len(a) <= THRESHOLD:
        return schoolMultiply(a, b)
    
    m = min(len(a), len(b)) / 2
    
    a_low  = a[0:m]
    a_high = a[m:]
    b_low  = b[0:m]
    b_high = b[m:]
    
    z0 = karatsuba(a_low, b_low)      // low × low
    z2 = karatsuba(a_high, b_high)    // high × high
    z1 = karatsuba(a_low + a_high, b_low + b_high) - z0 - z2  // cross term
    
    return z0 + (z1 << m) + (z2 << 2m)
```

### Implementation Steps

1. **Add helper method for splitting:**
```cpp
private:
    struct Split {
        BigInteger low;
        BigInteger high;
    };
    
    Split splitAt(size_t pos) const {
        Split result;
        result.low.digits.assign(digits.begin(), digits.begin() + std::min(pos, digits.size()));
        if (pos < digits.size()) {
            result.high.digits.assign(digits.begin() + pos, digits.end());
        }
        result.low.normalize();
        result.high.normalize();
        return result;
    }
```

2. **Add helper for left shift (multiply by BASE^n):**
```cpp
    BigInteger shiftLeft(size_t n) const {
        if (isZero() || n == 0) return *this;
        BigInteger result = *this;
        result.digits.insert(result.digits.begin(), n, 0);
        return result;
    }
```

3. **Implement Karatsuba multiply:**
```cpp
    static const size_t KARATSUBA_THRESHOLD = 64;
    
    BigInteger multiplyKaratsuba(const BigInteger& other) const {
        // Base case: use school method for small numbers
        if (digits.size() <= KARATSUBA_THRESHOLD || 
            other.digits.size() <= KARATSUBA_THRESHOLD) {
            return multiplySchool(other);
        }
        
        // Make lengths equal by padding
        size_t n = std::max(digits.size(), other.digits.size());
        size_t m = (n + 1) / 2;  // Split point
        
        // Split operands
        Split a = splitAt(m);
        Split b = other.splitAt(m);
        
        // Three recursive multiplications
        BigInteger z0 = a.low.multiplyKaratsuba(b.low);
        BigInteger z2 = a.high.multiplyKaratsuba(b.high);
        BigInteger z1 = (a.low + a.high).multiplyKaratsuba(b.low + b.high) - z0 - z2;
        
        // Combine: z0 + z1*BASE^m + z2*BASE^(2m)
        return z0 + z1.shiftLeft(m) + z2.shiftLeft(2 * m);
    }
```

4. **Update public multiply to use Karatsuba:**
```cpp
    BigInteger multiplyAbs(const BigInteger& other) const {
        return multiplyKaratsuba(other);
    }
    
    // Rename current implementation to multiplySchool()
    BigInteger multiplySchool(const BigInteger& other) const {
        // Current implementation from lines 225-251
    }
```

### Testing Strategy

```bash
# Test correctness first
python3 -c "
import random
for _ in range(100):
    a = random.randint(-10**5000, 10**5000)
    b = random.randint(-10**5000, 10**5000)
    print(f'{a}')
    print(f'{b}')
    print(f'{a * b}')
" > karatsuba_test.txt

# Compare with C++ implementation
./code < karatsuba_test.txt > cpp_output.txt
# Manual verification or diff
```

### Performance Tuning

- **THRESHOLD value:** Experiment with 32, 64, 128
  - Too low: Recursion overhead dominates
  - Too high: Doesn't benefit from optimization
  - Sweet spot usually around 64 for base 10^9

- **Memory optimization:** Reuse temporary objects instead of creating new ones

---

## Alternative: GMP Integration (0.2 days)

If external libraries are allowed, this is the fastest solution.

### Installation
```bash
# macOS
brew install gmp

# Ubuntu
sudo apt-get install libgmp-dev
```

### CMakeLists.txt modification
```cmake
find_package(GMP REQUIRED)
target_link_libraries(code PRIVATE gmp gmpxx)
```

### Implementation

Replace BigInteger class with thin wrapper:
```cpp
#include <gmpxx.h>

class BigInteger {
private:
    mpz_class value;  // GMP's arbitrary precision integer
    
public:
    BigInteger() : value(0) {}
    BigInteger(long long v) : value(v) {}
    BigInteger(const std::string& s) : value(s) {}
    
    BigInteger operator+(const BigInteger& o) const { 
        BigInteger r; r.value = value + o.value; return r; 
    }
    BigInteger operator-(const BigInteger& o) const { 
        BigInteger r; r.value = value - o.value; return r; 
    }
    BigInteger operator*(const BigInteger& o) const { 
        BigInteger r; r.value = value * o.value; return r; 
    }
    BigInteger floorDiv(const BigInteger& o) const { 
        BigInteger r; r.value = value / o.value; return r; 
    }
    BigInteger operator%(const BigInteger& o) const { 
        BigInteger r; r.value = value % o.value; return r; 
    }
    
    bool operator<(const BigInteger& o) const { return value < o.value; }
    bool operator==(const BigInteger& o) const { return value == o.value; }
    // ... other operators similar
    
    std::string toString() const { return value.get_str(); }
};
```

**Pros:** All performance issues solved, ~50 lines of code
**Cons:** External dependency, less educational

---

## Performance Testing Script

Create `benchmark_bigint.py`:
```python
#!/usr/bin/env python3
import random
import time
import subprocess

def generate_test(digit_count, operations):
    a = random.randint(10**(digit_count-1), 10**digit_count - 1)
    b = random.randint(10**(digit_count-1), 10**digit_count - 1)
    
    test = f"a = {a}\nb = {b}\n"
    for op in operations:
        test += f"print(a {op} b)\n"
    
    return test

def benchmark(digit_count, ops):
    test_input = generate_test(digit_count, ops)
    
    start = time.time()
    result = subprocess.run(['./code'], input=test_input, 
                          capture_output=True, text=True, timeout=10)
    elapsed = time.time() - start
    
    return elapsed, result.returncode == 0

# Test various sizes
for digits in [100, 500, 1000, 2000, 5000]:
    print(f"\n=== {digits} digits ===")
    for ops in [['+'], ['-'], ['*'], ['//'], ['%']]:
        try:
            elapsed, success = benchmark(digits, ops)
            status = "✓" if success else "✗"
            print(f"{ops[0]:3s}: {elapsed*1000:7.2f}ms {status}")
        except subprocess.TimeoutExpired:
            print(f"{ops[0]:3s}: TIMEOUT")
```

Run with:
```bash
chmod +x benchmark_bigint.py
./benchmark_bigint.py
```

---

## Decision Tree for Optimization

```
Is optimization needed?
├─ NO → Focus on functionality
└─ YES
   ├─ Is GMP allowed?
   │  ├─ YES → Use GMP (0.2 days)
   │  └─ NO → Continue
   ├─ Main bottleneck is division?
   │  ├─ YES → Implement division caching (0.5 days)
   │  └─ NO → Continue
   └─ Main bottleneck is multiplication?
      └─ YES → Implement Karatsuba (1-2 days)
```

---

## Common Pitfalls

1. **Sign handling:** Always work with absolute values, fix sign at end
2. **Normalization:** Call `normalize()` after every operation
3. **Edge cases:** Test with 0, 1, -1, equal numbers, opposite signs
4. **Off-by-one:** Be careful with split points in Karatsuba
5. **Memory leaks:** Ensure temporary objects are properly managed
6. **Integer overflow:** Use `unsigned long long` for intermediate products

---

## Quick Complexity Reference

| Algorithm | Addition | Multiplication | Division |
|-----------|----------|----------------|----------|
| Current | O(n) | O(n²) | O(n²) |
| + Division Cache | O(n) | O(n²) | O(n) |
| + Karatsuba | O(n) | O(n^1.585) | O(n^1.585) |
| + FFT | O(n) | O(n log n) | O(n log n) |
| GMP (auto) | O(n) | O(n log n)* | O(n log n)* |

*GMP automatically switches between algorithms based on input size

# BigInteger Implementation Research

## Key Requirements
Based on BigIntegerTest0.in, we need to handle integers with:
- 5000+ digits
- Addition, subtraction, multiplication, division (//)
- Negative numbers
- Comparison operators

## Storage Strategy Options

### 1. Vector of Digits (Base 10)
- Store each digit separately: vector<int> with values 0-9
- PROS: Easy to understand, simple string conversion
- CONS: Very inefficient (wastes 3+ bits per digit), slow arithmetic

### 2. Vector of Large Blocks (Base 10^k)
- Store blocks of k digits: e.g., base 10^9 in uint32_t
- PROS: Much more efficient than digit-by-digit, reasonable performance
- CONS: Need to handle overflow carefully

### 3. Vector of Binary Blocks (Base 2^32)
- Store in base 2^32 using vector<uint32_t>
- PROS: Maximum efficiency, natural for CPU
- CONS: Complex base conversion for I/O

**RECOMMENDATION: Option 2 (Base 10^9)**
- Good balance of efficiency and implementation complexity
- Each uint32_t can store up to 10^9 - 1
- Easier string I/O than binary
- Standard approach used in competitive programming

## Algorithm Complexity

### Addition/Subtraction: O(n)
- Straightforward digit-by-digit with carry

### Multiplication: O(n^2) or O(n log n)
- Simple: O(n^2) grade school multiplication - SUFFICIENT for our needs
- Advanced: Karatsuba O(n^1.585) - probably overkill
- FFT: O(n log n) - definitely overkill

For 5000 digit numbers:
- O(n^2) = 25M operations - easily under time limit
- Start with simple, optimize only if needed

### Division: O(n^2)
- Binary search on quotient digit + subtraction
- Complex but necessary
- Floor division must follow Python semantics: floor(a/b), not truncate

### Comparison: O(n)
- Lexicographic comparison of blocks

## Python-Specific Requirements

### Floor Division Semantics
C++: -5 / 3 = -1 (truncate toward zero)
Python: -5 // 3 = -2 (floor toward negative infinity)

Formula: floor(a/b) = (a - a%b) / b where % has same sign as divisor

### Modulo Semantics
Python: a % b = a - (a // b) * b
This means % result has same sign as divisor, NOT dividend

## Implementation Risks

1. **Sign handling** - Easy to get wrong with floor division
2. **Leading zeros** - Must normalize after operations
3. **Zero representation** - Should be consistent (positive zero)
4. **Division by zero** - Need to handle (though tests might not include)
5. **Memory allocation** - Large numbers need careful memory management

## Testing Strategy

1. Start with small numbers to verify logic
2. Test sign combinations systematically
3. Test edge cases: 0, 1, -1
4. Gradually increase to large numbers
5. Compare against Python output for validation


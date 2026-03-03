# String Multiplication Fix - Implementation Guide

## Problem
String multiplication (`"abc" * 3` or `3 * "abc"`) causes `bad_variant_access` crashes.

## Root Cause
`src/Evalvisitor.cpp::visitTerm()` doesn't handle string * int operations.

## Solution
Add string multiplication handling in `visitTerm()` before the numeric type checks.

## Code Location
File: `src/Evalvisitor.cpp`
Function: `std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx)`

## Implementation

### Step 1: Find the insertion point
Look for the line that starts: `std::string op = ops[i]->getText();`

After this line, there should be type checking with:
```cpp
if (std::holds_alternative<BigInteger>(result) || std::holds_alternative<BigInteger>(factor)) {
    // BigInteger handling
    ...
}
```

### Step 2: Add string multiplication BEFORE BigInteger check
Insert this code right after `std::string op = ops[i]->getText();`:

```cpp
        std::string op = ops[i]->getText();
        
        // ADD THIS BLOCK - String multiplication handling
        if (op == "*") {
            // Handle string * int
            if (std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) {
                std::string s = std::get<std::string>(result);
                int count = std::get<int>(factor);
                if (count <= 0) {
                    result = std::string("");
                } else {
                    std::string repeated;
                    repeated.reserve(s.length() * count);  // Optimize memory allocation
                    for (int i = 0; i < count; i++) {
                        repeated += s;
                    }
                    result = repeated;
                }
                continue;  // Skip to next iteration
            }
            
            // Handle int * string
            if (std::holds_alternative<int>(result) && std::holds_alternative<std::string>(factor)) {
                int count = std::get<int>(result);
                std::string s = std::get<std::string>(factor);
                if (count <= 0) {
                    result = std::string("");
                } else {
                    std::string repeated;
                    repeated.reserve(s.length() * count);  // Optimize memory allocation
                    for (int i = 0; i < count; i++) {
                        repeated += s;
                    }
                    result = repeated;
                }
                continue;  // Skip to next iteration
            }
        }
        // END OF ADDED BLOCK
        
        // Type coercion and operation
        // Handle BigInteger operations
        if (std::holds_alternative<BigInteger>(result) || std::holds_alternative<BigInteger>(factor)) {
            ...  // existing code
```

### Step 3: Handle string * BigInteger (optional but recommended)
Also add support for string * BigInteger (for very large repetitions):

```cpp
        // After the int * string block, add:
        
        if (op == "*") {
            // Handle string * BigInteger
            if (std::holds_alternative<std::string>(result) && std::holds_alternative<BigInteger>(factor)) {
                // For BigInteger counts, we need to check if it's too large
                // Python allows huge string repetitions but they can crash with memory
                std::string s = std::get<std::string>(result);
                BigInteger count_bi = std::get<BigInteger>(factor);
                
                // Check if count is negative or zero
                if (count_bi.isNegative() || count_bi.toString() == "0") {
                    result = std::string("");
                } else {
                    // Check if count is too large (would exceed memory)
                    // Let's limit to INT_MAX for safety
                    if (count_bi > BigInteger(INT_MAX)) {
                        throw std::runtime_error("String repetition count too large");
                    }
                    int count = std::stoi(count_bi.toString());
                    std::string repeated;
                    repeated.reserve(s.length() * count);
                    for (int i = 0; i < count; i++) {
                        repeated += s;
                    }
                    result = repeated;
                }
                continue;
            }
            
            // Handle BigInteger * string
            if (std::holds_alternative<BigInteger>(result) && std::holds_alternative<std::string>(factor)) {
                BigInteger count_bi = std::get<BigInteger>(result);
                std::string s = std::get<std::string>(factor);
                
                if (count_bi.isNegative() || count_bi.toString() == "0") {
                    result = std::string("");
                } else {
                    if (count_bi > BigInteger(INT_MAX)) {
                        throw std::runtime_error("String repetition count too large");
                    }
                    int count = std::stoi(count_bi.toString());
                    std::string repeated;
                    repeated.reserve(s.length() * count);
                    for (int i = 0; i < count; i++) {
                        repeated += s;
                    }
                    result = repeated;
                }
                continue;
            }
        }
```

## Test Cases

After implementing, test with these cases:

```bash
cd /path/to/repo

# Basic string multiplication
echo 'print("ab" * 3)' | ./code
# Expected: ababab

# Reverse order
echo 'print(3 * "ab")' | ./code
# Expected: ababab

# Zero repetitions
echo 'print("x" * 0)' | ./code
# Expected: (empty line)

# Negative repetitions
echo 'print("x" * -5)' | ./code
# Expected: (empty line)

# Variable multiplication
echo 's = "hello"
n = 2
print(s * n)' | ./code
# Expected: hellohello

# Large repetitions
echo 'print("x" * 1000)' | ./code
# Expected: xxxx... (1000 x's)

# BigInteger count
echo 'n = 10000000000
print("a" * n)' | ./code
# Expected: Error (count too large) OR very long string
```

## Alternative: More Efficient Implementation

For very large strings, the repeated concatenation can be slow. Here's a more efficient version:

```cpp
// More efficient string repetition
std::string repeatString(const std::string& str, int count) {
    if (count <= 0) return "";
    if (count == 1) return str;
    
    std::string result;
    result.reserve(str.length() * count);
    
    // Use doubling strategy for large counts
    std::string doubled = str;
    while (count > 0) {
        if (count & 1) {
            result += doubled;
        }
        count >>= 1;
        if (count > 0) {
            doubled += doubled;
        }
    }
    
    return result;
}

// Then use it:
if (std::holds_alternative<std::string>(result) && std::holds_alternative<int>(factor)) {
    std::string s = std::get<std::string>(result);
    int count = std::get<int>(factor);
    result = repeatString(s, count);
    continue;
}
```

## Expected Impact

- Fixes tests 34, 55, 72
- Pass rate: 54/72 → 57/72 (75% → 79.2%)
- **+3 tests, +4.2% improvement**

## Verification

After implementing:

1. Run local edge case tests:
   ```bash
   cd workspace/mia
   ./run_edge_tests.sh
   ```

2. Check that string multiplication tests now PASS instead of CRASH

3. Rebuild and test:
   ```bash
   cd /path/to/repo
   make clean
   make
   # Run tests 34, 55, 72 if you can access them
   ```

4. Submit to OJ and check if tests 34, 55, 72 now pass

## Additional Type Safety (Optional)

If you want to also handle invalid string operations gracefully:

```cpp
if (op == "*") {
    // After handling valid string * int cases, add:
    
    // Handle invalid string * string
    if (std::holds_alternative<std::string>(result) && std::holds_alternative<std::string>(factor)) {
        throw std::runtime_error("can't multiply sequence by non-int of type 'str'");
    }
}
```

This will give proper error messages instead of crashing for operations like `"a" * "b"`.

## Notes

- The `continue` statement is important - it skips the rest of the loop iteration after handling string multiplication
- Always check for negative and zero counts - Python returns empty string for these
- Reserve memory with `reserve()` for efficiency with large strings
- Consider memory limits for very large repetitions (billions of characters)

## Testing Checklist

- [ ] `"string" * positive_int` works
- [ ] `positive_int * "string"` works
- [ ] `"string" * 0` returns empty string
- [ ] `"string" * negative` returns empty string
- [ ] String variable * int variable works
- [ ] Large repetitions don't crash (e.g., 10000)
- [ ] Invalid operations raise proper errors (e.g., `"a" * "b"`)
- [ ] Tests 34, 55, 72 pass on OJ

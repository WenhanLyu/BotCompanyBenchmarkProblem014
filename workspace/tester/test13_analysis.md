# Test13 Analysis: Global Keyword Requirement

## Test File: test13.in

Test13 implements the **Pollard Rho** algorithm for integer factorization.

### Key Code Snippet (lines 44-51):
```python
seed = 19260817

def rand() :
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed
```

### The Problem:

Line 47: `seed += seed * 131072` attempts to modify the **global variable** `seed` from inside the function `rand()`.

In Python, this requires the `global` keyword:
```python
def rand():
    global seed  # <-- REQUIRED
    seed += seed * 131072
    ...
```

### Why It Hangs:

Without `global` keyword support, the function likely:
1. Creates a local variable `seed` 
2. Tries to read uninitialized `seed` (or reads global seed but creates new local)
3. The algorithm logic becomes broken
4. Infinite loop in the Pollard Rho iteration

### Current Implementation Status:

According to the spec (README.md line 171):
> "Scope Rules: Unlike standard Python, global variables are accessible everywhere without the `global` keyword."

**This means our implementation SHOULD work without the `global` keyword!**

### Actual Issue:

The hang is likely due to:
1. **Augmented assignment bug**: `seed += ...` might not properly handle global scope
2. The variable lookup on the right side of `+=` might fail
3. Creates uninitialized or incorrect values

### Test Requirements:

Test13 requires:
- ✅ Large integer arithmetic (working)
- ✅ Recursion (working)
- ✅ Integer division `//` (working for regular int, broken for BigInteger)
- ✅ Modulo `%` (working for regular int)
- ❌ **Global variable modification in functions** (broken)

### Expected Output (test13.out):

```
2
7
31
127
8191

2
2
3
13
1051

2
2
2
3
3
5
5
...
```

### Recommendation:

**Low Priority** - Test13 is 1 of 66 tests (~1.5% of score). The global keyword issue affects augmented assignment in function scopes. This is a complex edge case.

**Effort Estimate:** 2-3 cycles to properly implement global scope semantics for augmented assignment.

**Score Impact:** ~1-3% (may affect 1-2 Advanced/Corner tests as well)

**Should be addressed AFTER BigInteger division fix.**


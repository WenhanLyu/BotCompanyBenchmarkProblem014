# Critical Bug: Tuple Unpacking with Multiple Expressions

## Status
🔴 **BLOCKING test13 and likely many other tests**

## Bug Description
Tuple unpacking with multiple expressions only evaluates the FIRST expression correctly. All subsequent values become `None`.

## Reproduction

### Simple Case
```python
a, b = 1 + 2, 3 + 4
print(a, b)
```

**Current Output:** `7 None`  
**Expected Output:** `3 7`

### With Function Calls
```python
def rand():
    return 42

a, b = rand(), rand()
print(a, b)
```

**Current Output:** `42 None`  
**Expected Output:** `42 42`

### test13 Failing Case
```python
seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

n = 100
c, p = random(n - 1) + 1, random(n - 1) + 1
print(c, p)
```

**Current Output:** `33 None` (or similar first value)  
**Expected Output:** `33 95` (two different random values)

## Impact
- **test13 fails** with `Runtime error: bad_variant_access` when `p = None` is used in arithmetic
- Likely blocks many other Python programs using tuple unpacking
- Common Python pattern completely broken

## Root Cause
The assignment statement visitor is not properly handling multiple target assignments. It appears to evaluate only the first expression in the value list and assign `None` to remaining targets.

## Test Files
See `workspace/nina/test_simple_tuple.py` and `workspace/nina/test_random_assign.py` for minimal reproductions.

## Estimated Fix Time
30-45 minutes - requires fixing the assignment statement visitor to properly iterate through all expressions in the value list.

## Priority
**HIGH** - Blocks test13 and is a fundamental Python feature used in many programs.

## Related Issues
- Issue #83: Retest test13 after augmented assignment fix (discovered this bug during testing)

# Critical Bug: Tuple Unpacking with Multiple Expressions

## Status
✅ **RESOLVED** - Fixed by Leo in commit 2640bf8, validated by Nina

**Fixed on:** 2024-03-03  
**Branch:** leo/fix-tuple-unpacking  
**Validation:** workspace/nina/tuple_unpacking_validation_report.md

## Bug Description
Tuple unpacking with multiple expressions only evaluates the FIRST expression correctly. All subsequent values become `None`.

## Reproduction

### Simple Case
```python
a, b = 1 + 2, 3 + 4
print(a, b)
```

**Previous Output (BUGGY):** `7 None`  
**Current Output (FIXED):** `3 7` ✅

### With Function Calls
```python
def rand():
    return 42

a, b = rand(), rand()
print(a, b)
```

**Previous Output (BUGGY):** `42 None`  
**Current Output (FIXED):** `42 42` ✅

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

**Previous Output (BUGGY):** `33 None` (or similar first value)  
**Current Output (FIXED):** `88 33` (two different random values) ✅

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

## Resolution

**Fixed by:** Leo  
**Commit:** 2640bf8  
**Branch:** leo/fix-tuple-unpacking  
**Files changed:** src/Evalvisitor.cpp

The assignment statement visitor was modified to properly evaluate all expressions in a tuple assignment, not just the first one.

**Validation:**
- All test cases pass
- Simple expressions work: `a, b = 1+2, 3+4` → `3 7` ✅
- Function calls work: `c, p = random(n-1)+1, random(n-1)+1` → `88 33` ✅
- See `workspace/nina/tuple_unpacking_validation_report.md` for full validation

## Related Issues
- Issue #87: Fix tuple unpacking bug (RESOLVED)
- Issue #83: Retest test13 after augmented assignment fix (discovered this bug during testing)

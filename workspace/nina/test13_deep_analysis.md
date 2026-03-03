# test13 Deep Analysis - Why It Still Fails

## Summary

test13 produces 162 lines instead of 48 lines. The print() fix works correctly, but the factorization results are wrong.

## What Works ✅

1. **Print() fix** - Blank lines are output correctly
2. **BigInteger division** - No hanging, completes in 10 seconds
3. **Global variable handling** - The `seed` variable IS being updated correctly
4. **Random number generation** - The RNG produces identical sequences to Python

## Verification of Global Variables

Tested the exact RNG pattern from test13:
```python
seed = 19260817

def rand():
    seed += seed * 131072  # No "global" keyword
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed
```

**Result:** Our interpreter correctly updates `seed` across function calls, producing identical random numbers to Python (with global keyword added).

## The Mystery

If everything works individually:
- ✅ Print() outputs blank lines
- ✅ Global variables update correctly
- ✅ RNG produces correct sequence
- ✅ BigInteger operations work

Then why does test13 produce different factorizations?

## Hypothesis

The issue might be in the **interaction** between:
1. Recursive function calls (`pollard_rho` calls itself)
2. Multiple modifications to global variables during recursion
3. Tuple unpacking in the while loop: `c, p = random(n - 1) + 1, random(n - 1) + 1`

The RNG is called multiple times per iteration, and the ORDER of evaluation matters. If our interpreter evaluates tuple expressions in a different order than Python, the RNG calls happen in a different sequence, leading to different random values, which leads to different factorizations.

## Example

In line 66 of test13.in:
```python
c, p = random(n - 1) + 1, random(n - 1) + 1
```

Python evaluates:
1. First `random(n - 1) + 1` → call rand() once → seed updates
2. Second `random(n - 1) + 1` → call rand() again → seed updates again
3. Assign results to c, p

If our interpreter evaluates in different order (e.g., right-to-left), or caches values differently, the RNG calls happen in different order, producing different c and p values.

## Previous Issues

Issue #87 was about tuple unpacking bugs - it was supposedly fixed. But maybe there's still an edge case with:
- Tuple unpacking with function calls that modify global state
- Multiple function calls in one tuple assignment
- Order of evaluation in complex expressions

## Test Results

- Expected first factorization: 7, 9×2, 17, 13×2
- Actual first factorization: 7, 24×2, 17, 2, 17, ...

The first 7 is correct (it's a prime), but then the recursive factorizations diverge.

## Recommendation

The bug is NOT in:
- print() functionality
- Global variable handling
- Random number generation

The bug IS likely in:
- Expression evaluation order
- Tuple unpacking with side effects
- Recursive function behavior with global state

Need to investigate Evalvisitor.cpp for how tuple assignments with function calls are evaluated.

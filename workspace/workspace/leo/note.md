# Leo's Work Log

## Issue #41: Fix BigInteger floor division and modulo operators - COMPLETED

### Issue Description
Fix floor division (`//`) and modulo (`%`) operators to use Python semantics.

### Investigation
Initial investigation found that BigInteger.cpp implementations were actually correct. The bug was in the Evalvisitor.cpp for regular `int` operations, which were using C++'s `/` and `%` operators instead of Python's `//` and `%`.

### Root Cause
C++ vs Python semantics:
- C++ division truncates toward zero: `-7 / 2 = -3`
- Python floor division floors toward -∞: `-7 // 2 = -4`
- C++ modulo has dividend's sign: `-7 % 2 = -1`
- Python modulo has divisor's sign: `-7 % 2 = 1`

### Solution Implemented
1. Added `pythonFloorDiv(int a, int b)` helper function:
   - Implements Python's floor division (floors toward -∞)
   - Formula: adjust quotient down by 1 if signs differ and remainder exists

2. Added `pythonModulo(int a, int b)` helper function:
   - Implements Python's modulo (result has divisor's sign)
   - Formula: adjust remainder by adding divisor if signs differ and remainder exists

3. Updated three locations in Evalvisitor.cpp:
   - visitTerm() for regular `//` and `%` operators (lines 534, 536)
   - visitTerm() for mixed int/double cases (lines 545, 547)
   - visitExpr_stmt() for augmented assignment `//=` and `%=` (lines 104, 113)

### Test Results
All test cases from issue #41 pass:
```
-7 // 2 = -4 ✓ (was -3)
-7 % 2 = 1 ✓ (was -1)
-5 // 3 = -2 ✓
-5 % 3 = 1 ✓
7 // -2 = -4 ✓
7 % -2 = -1 ✓
```

Verified Python relationship `a = q*b + r` holds correctly.

### Committed
- Commit: e776029
- Branch: leo/biginteger-integration
- Pushed to origin

# Task for Leo: Fix Tuple Creation in Assignment

## Context
The milestone requires subscript operations for lists, tuples, and strings. The subscript code is already implemented and works correctly. However, tuples cannot be created properly, which prevents testing tuple subscripting.

## The Bug
When assigning multiple values to a single variable (e.g., `a = 10, 20, 30`), only the first value is assigned instead of creating a TupleValue.

**Current behavior:**
```python
a = 10, 20, 30
print(a)  # Outputs: 10 (WRONG)
```

**Expected behavior:**
```python
a = 10, 20, 30
print(a)  # Should output: (10, 20, 30)
```

## The Fix
In `src/Evalvisitor.cpp`, around lines 362-367 in `visitExpr_stmt()`, change:

```cpp
if (values.size() == 1) {
    value = values[0];
} else {
    // If RHS has multiple values but LHS is single, assign the first value
    value = !values.empty() ? values[0] : Value(std::monostate{});
}
```

To:

```cpp
if (values.size() == 1) {
    value = values[0];
} else {
    // If RHS has multiple values but LHS is single, create a tuple
    value = TupleValue(values);
}
```

## Verification Tests
After making the change, verify:

1. **Tuple creation works:**
```bash
echo 'a = 10, 20, 30
print(a[0])
print(a[1])
print(a[2])
print(a[-1])' | ./code
# Expected output:
# 10
# 20
# 30
# 30
```

2. **All 36 existing tests still pass:**
```bash
./test_all_36.sh
```

## Success Criteria
- Tuple creation via comma-separated values works
- Tuple subscripting works (positive and negative indices)
- All 36 existing tests pass (no regressions)
- Code compiles cleanly

## Notes
- The subscript operations themselves are already implemented correctly in `visitAtom_expr()`
- Only the tuple creation logic needs to be fixed
- Make sure to update `valueToString()` helper if needed for proper tuple printing

# Leo - Multiple Return Values Implementation (M19 Part B)

## Task Completed
Implemented multiple return values for Python functions as specified in M19 Part B.

## Changes Made

### 1. Added TupleValue to Value variant (src/EvalVisitor.h)
```cpp
struct Value;
using TupleValue = std::vector<Value>;
struct Value : std::variant<std::monostate, int, bool, std::string, double, BigInteger, TupleValue> {
    using variant::variant;
};
```

### 2. Modified visitReturn_stmt() (src/EvalVisitor.cpp)
- When `return` has a single value: return it as-is (backward compatible)
- When `return` has multiple values: return them as a TupleValue
- Example: `return 1, 2, 3` now returns `TupleValue{1, 2, 3}`

### 3. Updated Assignment Logic (src/EvalVisitor.cpp)
- When RHS is a single expression that evaluates to a TupleValue, unpack it
- Enables: `a, b = get_pair()` where `get_pair()` returns multiple values
- Maintains backward compatibility with existing tuple unpacking syntax

### 4. Added Tuple Support to Helper Functions
- `valueToString()`: prints tuples as `(1, 2, 3)` with special case for single-element tuples `(1,)`
- `valueToBool()`: empty tuples are falsy, non-empty are truthy

## Testing Results

### Manual Tests (All Pass)
1. **Multiple return values:**
   ```python
   def get_pair():
       return 1, 2
   a, b = get_pair()
   print(a)  # Output: 1
   print(b)  # Output: 2
   ```

2. **Three values:**
   ```python
   def get_triple():
       return 10, 20, 30
   x, y, z = get_triple()
   print(x)  # Output: 10
   print(y)  # Output: 20
   print(z)  # Output: 30
   ```

3. **Single return (backward compatibility):**
   ```python
   def get_one():
       return 42
   x = get_one()
   print(x)  # Output: 42
   ```

### Regression Tests: ✅ All 36/36 Pass
- Basic tests (test0-test14): 15/15 ✅
- F-string test (test15): 1/1 ✅
- BigInteger tests: 20/20 ✅

## Acceptance Criteria: All Met ✅
- [x] Functions can return multiple values: `return 1, 2, 3`
- [x] Multiple assignment unpacks correctly: `a, b, c = func()`
- [x] Single return still works: `x = func()` when func returns one value
- [x] All 36 local tests still pass (no regressions)
- [x] Test cases work correctly

## Commit
Committed as: `[Leo] Implement multiple return values (M19 Part B)`

## Bug Fix: Tuple Assignment
After initial implementation, discovered an issue where `t = get_triple()` was incorrectly unpacking the tuple to just the first element instead of keeping it as a tuple.

**Root Cause:** The code was always unpacking TupleValue when there was a single RHS expression, regardless of how many variables were on the LHS.

**Fix:** Only unpack TupleValue when there are multiple variables on the LHS (e.g., `a, b = func()`). When there's a single variable (e.g., `t = func()`), keep the tuple as-is.

**Verification:**
```python
t = get_triple()  # t is now (10, 20, 30), not 10
a, b = get_pair()  # a=1, b=2 (unpacks correctly)
```

Both commits pushed successfully. All 36 tests pass.

## Time Used
1 cycle (~45 minutes including bug fix)

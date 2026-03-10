# Task for Leo: Implement Multiple Return Values (M19 Part B)

## Problem

Functions can only return a single value. When you do `return 1, 2`, only the first value (1) is returned, and the second is lost.

**Current Broken Behavior:**
```python
def get_pair():
    return 1, 2

a, b = get_pair()
print(a)  # Prints: 1
print(b)  # Prints: None (WRONG! Should be 2)
```

**Expected Python Behavior:**
```python
a, b = get_pair()
print(a)  # Should print: 1
print(b)  # Should print: 2
```

## Root Cause

In `src/EvalVisitor.cpp`, `visitReturn_stmt()` at line 1653 only returns `tests[0]`:

```cpp
auto tests = testlist->test();
if (!tests.empty()) {
    auto result = visit(tests[0]);  // ← BUG: Only returns first value!
    // ...
}
```

## Solution

Modify `visitReturn_stmt()` to handle multiple return values:

1. **If only one value:** Return it as-is (maintain backward compatibility)
2. **If multiple values:** Return them as a tuple (TupleValue)

### Implementation Steps

1. **Add TupleValue to Value variant** (if not already present):
   ```cpp
   using TupleValue = std::vector<Value>;
   using Value = std::variant<std::monostate, int, bool, std::string, double, BigInteger, TupleValue>;
   ```

2. **Modify visitReturn_stmt()** (around line 1643):
   ```cpp
   auto tests = testlist->test();
   if (tests.size() == 1) {
       // Single value - return as-is
       auto result = visit(tests[0]);
       // ... existing logic ...
   } else if (tests.size() > 1) {
       // Multiple values - return as tuple
       TupleValue tuple;
       for (auto test : tests) {
           auto result = visit(test);
           if (result.has_value()) {
               tuple.push_back(std::any_cast<Value>(result));
           }
       }
       returnValue = Value(tuple);
   }
   ```

3. **Update tuple unpacking in visitExpr_stmt()** (assignment):
   - When assigning `a, b = func()`, check if RHS is a TupleValue
   - If yes, unpack each element to the corresponding variable
   - This might already work if tuple unpacking is implemented

### Test Cases

**Test 1: Basic multiple return**
```python
def get_pair():
    return 1, 2

a, b = get_pair()
print(a)  # Should print: 1
print(b)  # Should print: 2
```

**Test 2: Three values**
```python
def get_triple():
    return 10, 20, 30

x, y, z = get_triple()
print(x)
print(y)
print(z)
```

**Test 3: Single return (backward compatibility)**
```python
def get_one():
    return 42

x = get_one()
print(x)  # Should print: 42
```

## Acceptance Criteria

- [ ] Functions can return multiple values: `return 1, 2, 3`
- [ ] Multiple assignment unpacks correctly: `a, b, c = func()`
- [ ] Single return still works: `x = func()` when func returns one value
- [ ] All 36 local tests still pass (no regressions)
- [ ] Test cases above work correctly

## Files to Modify

- `src/EvalVisitor.cpp` - visitReturn_stmt() implementation
- `src/Evalvisitor.h` - Value type definition (if TupleValue not present)
- Possibly assignment handling if tuple unpacking needs work

## Expected Time

1 cycle (30-60 minutes of focused work)

## Priority

HIGH - This is M19 Part B, the current milestone. String multiplication (Part A) is already done.

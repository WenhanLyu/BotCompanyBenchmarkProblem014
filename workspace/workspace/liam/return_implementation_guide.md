# Return Statement Implementation Guide

**Author:** Liam  
**Date:** 2026-03-02  
**For:** Developer implementing return statements

## Quick Reference

**Files to modify:**
- `src/Evalvisitor.h` - Add ReturnException class
- `src/Evalvisitor.cpp` - Implement visitReturn_stmt() and modify function execution

**Estimated effort:** 2-3 cycles  
**Complexity:** MEDIUM

## Implementation Steps

### Step 1: Add ReturnException Class (Evalvisitor.h)

Add after the existing BreakException and ContinueException classes:

```cpp
class ReturnException : public std::exception {
public:
    Value value;
    ReturnException(Value v) : value(v) {}
};
```

### Step 2: Implement visitReturn_stmt() (Evalvisitor.cpp)

```cpp
std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    if (ctx->testlist()) {
        // Return with value
        Value returnValue = std::any_cast<Value>(visit(ctx->testlist()));
        throw ReturnException(returnValue);
    } else {
        // Bare return - return None
        throw ReturnException(Value(std::monostate{}));
    }
    return std::any();
}
```

### Step 3: Modify Function Call Execution (Evalvisitor.cpp)

Find the code in `visitAtom()` or wherever function calls are handled and wrap the function body execution with try-catch:

```cpp
// Execute function body
try {
    // Visit all statements in function body
    for (auto stmt : funcBody) {
        visit(stmt);
    }
    // If no return statement, return None
    returnValue = Value(std::monostate{});
} catch (const ReturnException& e) {
    // Return statement executed
    returnValue = e.value;
} catch (const BreakException& e) {
    // Break outside loop - this is an error in well-formed code
    // But for robustness, could handle or rethrow
    throw;
} catch (const ContinueException& e) {
    // Continue outside loop - this is an error in well-formed code
    throw;
}
```

### Step 4: Handle Tuple Returns (if not already working)

The testlist might return a tuple. Ensure the Value type can handle tuples, or extract individual values as needed.

From test cases, it appears multiple return values are used like:
```python
return a, b  # Returns a tuple
```

Check if this is already handled by visitTestlist().

## Testing Strategy

### Phase 1: Simple Return Tests

Create `workspace/liam/test_return_simple.py`:

```python
# Test 1: Simple return
def foo():
    return 42

print(foo())

# Test 2: Return None
def bar():
    return

print(bar())

# Test 3: No return statement
def baz():
    x = 1

print(baz())

# Test 4: Return expression
def calc(a, b):
    return a + b

print(calc(3, 4))
```

Expected output:
```
42
None
None
7
```

### Phase 2: Early Return Tests

Create `workspace/liam/test_return_early.py`:

```python
def find_positive(x):
    if x > 0:
        return x
    return -x

print(find_positive(5))
print(find_positive(-5))
```

Expected output:
```
5
5
```

### Phase 3: Recursive Return Tests

Create `workspace/liam/test_return_recursive.py`:

```python
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

print(factorial(5))
print(factorial(10))
```

Expected output:
```
120
3628800
```

### Phase 4: test13 Validation

Once basic returns work, test with the full test13.in:

```bash
./code < testcases/basic-testcases/test13.in > test13_actual.out
diff test13_actual.out workspace/liam/test13_reference.out
```

Expected: No differences (exact match).

## Edge Cases to Consider

1. **Return in nested functions**
   ```python
   def outer():
       def inner():
           return 1
       return inner()
   ```

2. **Return in loops**
   ```python
   def find_first_even(nums):
       for n in nums:
           if n % 2 == 0:
               return n
       return None
   ```

3. **Multiple returns in same function**
   ```python
   def classify(x):
       if x > 0:
           return "positive"
       elif x < 0:
           return "negative"
       else:
           return "zero"
   ```

4. **Return with complex expressions**
   ```python
   def complex_calc(a, b, c):
       return a * b + c * 2 - (a + b) // 2
   ```

## Common Pitfalls

1. **Don't forget to handle bare return** (return without value = return None)
2. **Exception must be caught at the right level** (function call level, not statement level)
3. **Scope cleanup** - Ensure local scope is properly destroyed even when exception is thrown
4. **Break/Continue interaction** - Return should take precedence over break/continue

## Verification Checklist

- [ ] ReturnException class added to Evalvisitor.h
- [ ] visitReturn_stmt() implemented
- [ ] Function call execution wrapped with try-catch
- [ ] Simple return test passes
- [ ] Bare return (return None) works
- [ ] Early return works
- [ ] Recursive return works
- [ ] test13.in produces exact match with reference output
- [ ] No regressions on test0-12
- [ ] No memory leaks (run with valgrind)

## Performance Considerations

Exception-based control flow is efficient for normal cases (no early return) but has overhead when exceptions are thrown. However:

1. This is consistent with break/continue already implemented
2. Return statements are relatively rare compared to normal execution
3. The overhead is negligible compared to other operations

For test13 (Pollard Rho), the bottleneck is arithmetic operations, not return statements.

## References

- Grammar: `resources/Python3Parser.g4` line 22: `return_stmt: 'return' (testlist)?;`
- Documentation: `docs/grammar.md` Section 9.5 Jump Statements
- Test case: `testcases/basic-testcases/test13.in` (uses return extensively)
- Reference output: `workspace/liam/test13_reference.out`

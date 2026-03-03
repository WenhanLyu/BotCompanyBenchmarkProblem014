# Leo's Notes - Issue #94: Fix Variable Scoping

## Task
Fix the variable scoping issue that causes test13 to fail.

## Problem Analysis

### Current Behavior (WRONG)
Our interpreter treats all variables as global:
- When a function assigns to a variable, it modifies the global variable
- Example:
  ```python
  x = 10
  def f():
      x = 5  # This modifies the global x
  f()
  print(x)  # Prints 5 (WRONG - should be 10)
  ```

### Expected Python Behavior
Python uses proper local/global scoping:
- Variables assigned in a function are LOCAL by default
- Variables only read are looked up in global scope
- Augmented assignment (`x += 5`) counts as both read and write
- Example:
  ```python
  x = 10
  def f():
      x = 5  # This creates a LOCAL x
  f()
  print(x)  # Prints 10 (correct)
  ```
  
### Why test13 Fails
test13.in has a `rand()` function that tries to modify `seed`:
```python
seed = 19260817
def rand():
    seed += seed * 131072  # Should fail without 'global seed'
    ...
```

In standard Python, this gives `UnboundLocalError` because:
1. `seed += ...` is an assignment, making `seed` local
2. But the right side reads `seed` before it's assigned locally

Our interpreter incorrectly allows this by treating `seed` as global.

## Solution Plan

Implement proper Python scoping WITHOUT requiring `global` keyword (grammar doesn't support it):

1. **During function definition**: Scan the function body to find all assigned variables
2. **During function execution**: 
   - Create a separate local variable map
   - For reads: check local first, then global
   - For writes: write to local if variable is in assigned set
   - For augmented assignment: check local exists first

### Implementation Steps

1. Add method `findAssignedVariables(suite)` to scan a function body
2. Store assigned variable names in `FunctionDef` struct
3. Update function call logic to:
   - Create local scope map
   - Handle reads/writes with proper scoping
   - Throw error for augmented assignment on uninitialized locals

## Files to Modify
- `src/Evalvisitor.h` - Add local scope tracking to FunctionDef
- `src/Evalvisitor.cpp` - Implement scoping logic

## Status
Ready to implement the fix.

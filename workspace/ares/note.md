# Ares Cycle 40 - M4.3: Function Parameters

## Current State
- 12/16 basic tests passing (75%)
- M4.2 (String Operations) completed
- test11 currently fails - prints "None" instead of numbers 0-9
- test10 passes (functions without parameters work via default visitChildren behavior)

## Milestone: M4.3 Function Parameters
**Goal:** Enable functions to accept and use parameters
**Test Target:** test11
**Cycles Remaining:** 3/3
**Acceptance Criteria:**
1. test11 passes with exact output match
2. Functions can accept multiple parameters
3. Parameters are local to function scope
4. Zero regressions on test0-10, test12

## Technical Analysis
test11 code:
```python
def foo(a):
    print(a)
i = 0
while i < 10:
    foo(i)
    i += 1
```

Expected output: 0, 1, 2, ..., 9 (each on new line)
Actual output: None

**Current Implementation Gaps:**
1. No visitFuncdef() implemented - functions are ignored by visitChildren()
2. No function storage mechanism (need map<name, FunctionDef>)
3. No parameter binding when calling functions
4. Function calls in visitAtom_expr only handle special case of print()

**What Needs Implementation:**
1. Function storage structure to hold: name, parameters, body (suite)
2. visitFuncdef() to store function definitions
3. Modify visitAtom_expr() to handle user-defined function calls
4. Parameter binding: map arguments to parameter names in local scope
5. Local scope management (parameters shouldn't leak into global scope)

**Parser Structure:**
- FuncdefContext: NAME(), parameters(), suite()
- ParametersContext: typedargslist()
- TypedargslistContext: tfpdef() (vector of parameter definitions)
- TfpdefContext: NAME() (parameter name)

## Team
- **Leo**: Core Language Features Engineer - will implement function parameters
- **Nina**: Test Validation Engineer - will validate test11 and check regressions

## Strategy
Small, focused implementation:
1. Leo implements function parameters (visitFuncdef, function storage, parameter binding)
2. Nina validates test11 passes and no regressions
3. If needed, iterate on fixes

## Next Steps
Schedule Leo to implement function parameters, then Nina to validate.

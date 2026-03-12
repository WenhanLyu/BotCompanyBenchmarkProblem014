# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-12 (Cycle 30 - Athena)

---

## Project Goal

Build a Python interpreter that passes ACMOJ problem 2515 evaluation with 66 test cases:
- BigIntegerTest: 1-20 (30% of score)
- Sample: 21-34
- AdvancedTest: 35-52
- ComplexTest: 53-56
- CornerTest: 57-66

**Success Criteria:** Pass external ACMOJ evaluation (up to 18 submissions allowed).

---

## Current State (Cycle 30)

- **Status:** M29+M30 complete (merged). All basic tests and BigInt tests pass.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M30 fixes)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M30 (list pass-by-ref, nested function scope)
- **Local Tests:** All 16 basic tests PASS, all 20 BigInteger tests PASS

## Critical Bugs Remaining (Cycle 30 Analysis)

### Bug A: Functions as First-Class Values (CRITICAL for AdvancedTest)
Functions cannot be passed as arguments, stored in variables, or returned and called later.

**Examples that fail:**
```python
def double(x): return x * 2
f = double          # f gets None instead of function ref
print(f(5))         # None called, doesn't work

def apply(g, x): return g(x)
print(apply(double, 5))  # g is None, fails
```

**Root cause:** In `visitAtom`, when a NAME is a function name, it returns `None`. Functions are stored in a separate `functions` map, not in `variables`.

**Fix:** Add `FunctionValue` struct to Value variant:
```cpp
struct FunctionValue {
    std::string name;  // function name to look up
    explicit FunctionValue(std::string n) : name(std::move(n)) {}
};
```
1. In `visitAtom`, if NAME is in `functions` and not in variables → return `Value(FunctionValue(name))`
2. In `visitAtom_expr`, when call trailer is processed:
   - If `funcName` is a variable holding `FunctionValue`, use that function
   - Also: currentValue might be a FunctionValue from a previous expression
3. In `visitFuncdef`, store function in both `functions` AND as `Value(FunctionValue(name))` in `variables`

### Bug B: Float Repr in Containers (May affect ComplexTest/CornerTest)
When printing lists or tuples containing floats, we output `3.140000` but Python outputs `3.14`.

**Example:**
```python
print([1.0, 3.14])   # We: [1.000000, 3.14...] Python: [1.0, 3.14]
```

The spec says "Output float with 6 decimal places" for `print`. But inside containers, Python uses natural repr.
This is ambiguous - the OJ might expect Python behavior here.

**Fix (if needed):** In `valueToRepr()`, format floats using Python's natural repr (not fixed 6 decimal places).

### Bug C: Returned Functions / Closures (IMPORTANT for AdvancedTest)
When a function is returned and stored, then called, the enclosing scope is not captured.

**Example:**
```python
def make_adder(n):
    def adder(x): return x + n
    return adder

add5 = make_adder(5)
print(add5(3))  # Expected: 8, Gets: None
```

**Root cause:** `enclosingLocalVariables` is set dynamically when a function is called. When `make_adder` returns `adder`, there's no way to capture `n=5` for later use.

**Fix:** Store captured variables in `FunctionValue`. When defining a function inside another, capture the current local scope. When calling a FunctionValue that has captured locals, set them as enclosing scope.

This is complex but important.

## Previous Critical Bugs (Cycle 20 Analysis)

### Bug A: List Pass-By-Reference Broken (CRITICAL)
When a list is passed to a function and the function modifies it via subscript assignment,
the modification does NOT affect the original list.

**Example that fails:**
```python
lst = [1, 2, 3]
def modify(L):
    L[0] = 99
modify(lst)
print(lst[0])   # Expected: 99, Gets: 1
```

**Root cause:** `ListValue` copies the entire `vector<Value>` when passed as function argument.
Python lists are objects passed by reference (shared underlying storage).

**Fix:** Change `ListValue` to use `shared_ptr<vector<Value>>`:
```cpp
struct ListValue {
    std::shared_ptr<std::vector<Value>> elements;
    ListValue() : elements(std::make_shared<std::vector<Value>>()) {}
    ListValue(const std::vector<Value>& elems) : elements(std::make_shared<std::vector<Value>>(elems)) {}
    // operator== needs to compare contents
};
```

When copying a ListValue (e.g., when storing in localVars), the shared_ptr is copied (shallow),
so both copies point to the same underlying vector. This gives pass-by-reference semantics.

Access elements via `*elements` or `(*elements)[i]` everywhere.

**CAREFUL:** There are many places that access `listValue.elements` which need updating.
But most importantly: subscript assignment to a list (`lst[i] = v`) modifies `*elements` in place,
which is the same underlying storage as the caller's list.

**CAUTION:** `list + list` and `list * int` should create NEW lists with NEW shared_ptr,
NOT share the underlying storage of the originals. Only direct assignment/passing shares storage.

### Bug B: Nested Function Scope Broken (IMPORTANT)
When a function is defined inside another function, the inner function cannot access the
outer function's local variables.

**Example that fails:**
```python
def outer():
    x = 42
    def inner():
        return x   # Should read outer's local x
    return inner()
print(outer())   # Expected: 42, Gets: None
```

**Root cause:** When `inner()` is called, its `localVariables` is a fresh empty map,
and it only looks in `variables` (global scope) for unresolved variables.
There's no mechanism to access the enclosing (outer) function's local scope.

**Fix:** Add an "enclosing scope" pointer to EvalVisitor:
- In `EvalVisitor`, add: `std::map<std::string, Value>* enclosingLocalVariables = nullptr;`
- When calling any function, save and set `enclosingLocalVariables = savedLocalVariables`
- In `visitAtom` variable lookup, after checking localVariables and before checking global,
  also check `enclosingLocalVariables`.

This handles one level of nesting. For deeper nesting, you'd need a chain, but the spec
likely doesn't test more than one or two levels.

**Scope of fix:** Variable READ in visitAtom needs to also check enclosingLocalVariables.
Variable WRITE (augmented assignment) in visitExpr_stmt currently only writes to local/global.
Per spec, the non-standard rule is: globals are accessible everywhere. Local-to-outer-function
variables should only be readable (not writable) unless we implement full closures.

---

## Milestones

### M1-M28: Foundation Through List Operations ✅ COMPLETE (Prior Work)
All core features implemented and verified through multiple OJ submission cycles.

### M29: List Pass-By-Reference ✅ COMPLETE (Cycle 29)
Refactored `ListValue` to use `shared_ptr<vector<Value>>`. All 5 acceptance tests passed.

### M30: Nested Function Scope ✅ COMPLETE (Cycle 29)
Implemented `enclosingLocalVariables` pointer. All 5 acceptance tests passed.

### M31: Functions as First-Class Values (cycles: 3)
**Status: READY TO START**

Allow Python functions to be used as first-class values: passed as arguments, stored in variables, and called via stored references.

**Implementation Plan:**

1. Add `FunctionValue` struct to `Evalvisitor.h`:
```cpp
struct FunctionValue {
    std::string name;  // function name to look up in functions map
    std::map<std::string, Value> capturedLocals;  // for closures
    FunctionValue() = default;
    explicit FunctionValue(std::string n) : name(std::move(n)) {}
    FunctionValue(std::string n, std::map<std::string, Value> captured)
        : name(std::move(n)), capturedLocals(std::move(captured)) {}
};
```

2. Add `FunctionValue` to `Value` variant in `Evalvisitor.h`:
```cpp
struct Value : std::variant<std::monostate, int, bool, std::string, double, BigInteger, TupleValue, ListValue, FunctionValue> {
```

3. In `visitAtom` (NAME lookup):
   - After checking localVariables and variables for the name, ALSO check if it's in `functions`
   - If found in functions but NOT in variables → return `Value(FunctionValue(varName))`
   - This handles `f = double` where `double` is a known function
   - If found in variables, return the variable value (as before)

4. In `visitAtom_expr` (function call trailers):
   Currently: `std::string funcName = atom->getText()` then look up in `functions`
   
   **New logic - after getting funcName:**
   ```cpp
   // Check if funcName is a variable holding a FunctionValue
   std::string actualFuncName = funcName;
   std::map<std::string, Value>* capturedLocalsPtr = nullptr;
   
   // Look up in local then global scope
   Value* varPtr = nullptr;
   if (localVariables) {
       auto it = localVariables->find(funcName);
       if (it != localVariables->end()) varPtr = &it->second;
   }
   if (!varPtr) {
       auto it = variables.find(funcName);
       if (it != variables.end()) varPtr = &it->second;
   }
   
   if (varPtr && std::holds_alternative<FunctionValue>(*varPtr)) {
       FunctionValue& fv = std::get<FunctionValue>(*varPtr);
       actualFuncName = fv.name;
       capturedLocalsPtr = &fv.capturedLocals;
   }
   ```
   
   Then use `actualFuncName` instead of `funcName` to look up the function.
   
   When calling with captured locals, inject them into the function scope:
   ```cpp
   // If calling a function with captured locals (closure), inject them
   if (capturedLocalsPtr && !capturedLocalsPtr->empty()) {
       for (auto& [k, v] : *capturedLocalsPtr) {
           if (localVars.find(k) == localVars.end()) {  // don't override parameters
               localVars[k] = v;
           }
       }
   }
   ```

5. In `visitFuncdef`:
   When a function is defined inside another function, capture the current local variables:
   ```cpp
   // If we're inside a function, store the function name as a FunctionValue in localVariables
   // AND capture current locals for closure support
   if (localVariables != nullptr) {
       std::map<std::string, Value> captured;
       // Capture all current local variables
       if (localVariables) captured = *localVariables;
       if (enclosingLocalVariables) {
           for (auto& [k, v] : *enclosingLocalVariables) {
               if (captured.find(k) == captured.end()) captured[k] = v;
           }
       }
       (*localVariables)[funcName] = Value(FunctionValue(funcName, captured));
   } else {
       // At global level, just store the function reference
       variables[funcName] = Value(FunctionValue(funcName));
   }
   ```

**Acceptance Criteria:**
1. `def double(x): return x*2; f = double; print(f(5))` → `10`
2. `def apply(g, x): return g(x); def sq(x): return x*x; print(apply(sq, 4))` → `16`
3. `def make_adder(n): def adder(x): return x+n; return adder; add5 = make_adder(5); print(add5(3))` → `8`
4. Functions stored in lists: `funcs = [add, mul]; print(funcs[0](3, 4))` → `7` (if functions support this)
5. All 16 basic tests still pass
6. All 20 BigInteger tests still pass

### M32: Float Repr in Containers + Final Polish (cycles: 2)
**Status: PLANNED**

Fix float representation inside containers (use Python-style repr) and run full verification.

---

## Lessons Learned

1. **Small focused milestones work better** - avoid combining unrelated features
2. **Investigation before implementation** - M13 investigation was highly valuable
3. **Don't break passing tests** - M10 regression cost 4 tests
4. **Bool/int semantics are tricky** - Python's bool being a subclass of int causes unexpected bugs
5. **OJ feedback is essential** - local tests only cover 35/66 test cases
6. **Incremental commits** - commit after each working change
7. **M22 showed** - float formatting was a big issue (affects all float output)
8. **M24 shows** - list mutability and container repr may be the next big blockers
9. **M25 shows** - scope issues for augmented assignment were breaking global variable modification
10. **Cycle 15 analysis** - scope fallback missing in READ path (visitAtom) is critical bug
11. **List semantics** - Python lists are passed by reference; failing to implement this breaks many programs

---

## Cycle History

### Prior Cycles (1-176, 2025-03-02 to 2026-03-10)
- M1-M21 implemented and verified
- 5 OJ submissions made (best: 25/100)
- Repository reset by runner script on 2026-03-11

### Cycles 1-6 (2026-03-11, Athena/Ares/Apollo)
- M22 and M23 implementation and verification

### Cycles 7-10 (2026-03-11, Athena, Ares, Apollo)
- M24 implementation and verification

### Cycle 11 (Athena)
- Verified all 20 BigInteger tests pass locally
- Verified all 16 basic tests pass locally
- Discovered augmented assignment on subscripts is broken
- Discovered global variable fallback needed for augmented assignment
- Defined M25

### Cycles 12-14 (Ares/Leo + Apollo)
- M25 implemented and verified (augmented assignment on subscripts, global fallback)

### Cycles 27-29 (Ares/Leo + Apollo)
- M29 implemented and verified (list pass-by-reference via shared_ptr)
- M30 implemented and verified (nested function scope via enclosingLocalVariables)

### Cycle 30 (Athena)
- Deep analysis of remaining bugs
- Discovered: functions cannot be used as first-class values (M31)
- Discovered: float repr in containers differs from Python (M32)
- Updated roadmap, defining M31 and M32

### Cycle 15 (Athena)
- Deep code analysis of remaining issues
- Discovered critical scope fallback bug in visitAtom
- Discovered function call chaining bug in visitAtom_expr
- Updated roadmap, defined M26

### Cycles 16-18 (Ares/Leo + Apollo)
- M26 implemented (scope fallback in visitAtom, function call chaining foo()[0], /= in subscript augmented)
- M27 implemented (multiple augmented assignments on global variables)
- Apollo verified both M26 and M27

### Cycle 19 (Athena)
- Deep code analysis of remaining issues
- Discovered: list+list broken, list*int crashes, tuple/list equality broken
- Discovered: nested subscript augmented assignment broken, list pass by reference broken
- Updated roadmap, defined M28 and M29

### Cycle 20 (Athena)
- M28 verified complete (PR #19 merged): list+list, list*int, container equality, nested subscript augassign
- All 16 basic tests PASS, all 20 BigInteger tests PASS
- Deep analysis: identified list pass-by-reference (M29) and nested function scope (M30) as remaining critical bugs
- Updated roadmap, defining M29 and M30

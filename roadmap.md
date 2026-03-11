# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-11 (Cycle 20 - Athena)

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

## Current State (Cycle 20)

- **Status:** M28 complete (PR #19 merged). All basic tests and BigInt tests pass.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M28 fixes)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M28 (list concat, list repeat, container equality, nested subscript augassign)
- **Local Tests:** All 16 basic tests PASS, all 20 BigInteger tests PASS

## Critical Bugs Remaining (Cycle 20 Analysis)

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

### M29: List Pass-By-Reference (cycles: 3)
**Status: READY TO START**

Refactor `ListValue` to use `shared_ptr<vector<Value>>` for pass-by-reference semantics.

**Implementation Plan:**
1. Change `ListValue` struct to use `shared_ptr`:
```cpp
struct ListValue {
    std::shared_ptr<std::vector<Value>> elements;
    ListValue() : elements(std::make_shared<std::vector<Value>>()) {}
    ListValue(const std::vector<Value>& elems) : elements(std::make_shared<std::vector<Value>>(elems)) {}
    bool operator==(const ListValue& other) const;
    bool operator!=(const ListValue& other) const;
};
```

2. Update ALL accesses to `listValue.elements` to use `*listValue.elements` or `listValue.elements->...`
   Key locations:
   - `visitAtom`: creating list literal → use `ListValue(elements)` constructor
   - `visitArith_expr`: list + list → create NEW ListValue with combined elements
   - `visitTerm`: list * int → create NEW ListValue with repeated elements
   - `visitAtom_expr`: subscript access `lst[i]` → access `(*listValue.elements)[i]`
   - `visitExpr_stmt`: subscript assignment `lst[i] = v` → modify `(*listValue.elements)[i]`
   - `valueToString` / `valueToRepr`: iterate `*listValue.elements`
   - `TupleValue::operator==`, `ListValue::operator==`: compare `*elements`
   - everywhere else that accesses `.elements`

3. The key semantic: when a ListValue is COPIED (stored in localVars), the shared_ptr is shallow-copied,
   so both copies share the same underlying vector. Modifications via subscript affect the original.

4. `list + list` and `list * int` must create DEEP COPY (new shared_ptr) to avoid aliasing:
```cpp
// list + list: create new independent list
ListValue combined;
for (auto& e : *l.elements) combined.elements->push_back(e);
for (auto& e : *r.elements) combined.elements->push_back(e);
```

**Acceptance Criteria:**
1. `lst = [1,2,3]; def f(L): L[0] = 99; f(lst); print(lst[0])` → `99`
2. `m = [[1,2],[3,4]]; def f(M): M[0][0] = 99; f(m); print(m[0][0])` → `99`
3. `a = [1,2]; b = a + [3]; a[0] = 99; print(b[0])` → `1` (a+b creates new list)
4. All 16 basic tests still pass
5. All 20 BigInteger tests still pass

### M30: Nested Function Scope (cycles: 2)
**Status: PLANNED (after M29)**

Implement enclosing scope lookup so nested functions can read outer function's local variables.

**Implementation:**
1. Add `enclosingLocalVariables` pointer to EvalVisitor private members
2. In function call execution (visitAtom_expr):
   ```cpp
   std::map<std::string, Value>* savedEnclosing = enclosingLocalVariables;
   enclosingLocalVariables = savedLocalVariables;  // outer's locals become enclosing
   localVariables = &localVars;
   // ... execute body ...
   localVariables = savedLocalVariables;
   enclosingLocalVariables = savedEnclosing;
   ```
3. In `visitAtom` variable lookup, after checking localVariables, check enclosingLocalVariables:
   ```cpp
   // Check enclosing scope (for nested functions)
   if (enclosingLocalVariables != nullptr) {
       auto enclosingIt = enclosingLocalVariables->find(varName);
       if (enclosingIt != enclosingLocalVariables->end()) {
           return enclosingIt->second;
       }
   }
   ```

**Acceptance Criteria:**
1. `def outer(): x=42; def inner(): return x; return inner(); print(outer())` → `42`
2. `def make_adder(n): def adder(x): return x+n; return adder(5); print(make_adder(3))` → `8`
3. All previous tests still pass

### M31: Final Polish and Submit (cycles: 2)
**Status: PLANNED**

Run full test suite, verify correctness, merge to master, mark ready for OJ evaluation.

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

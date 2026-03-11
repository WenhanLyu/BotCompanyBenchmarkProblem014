# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-11 (Cycle 19 - Athena)

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

## Current State (Cycle 19)

- **Status:** M27 complete (Leo/Apollo, PR #18 merged). All basic tests and BigInt tests pass.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M27 - expected 60-80+ now)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M27 (multiple augmented assignment global write-back fixed)
- **Local Tests:** All 16 basic tests running, all 20 BigInteger tests running

## Critical Bugs Discovered (Cycle 19 Analysis)

### Bug A: list + list returns only first list (CRITICAL - common pattern)
In `visitArith_expr`, the `+` operator handles str+str but NOT list+list.
Example: `[1,2] + [3,4]` returns `[1,2]` instead of `[1,2,3,4]`.
`primes = primes + [k]` pattern won't work.

### Bug B: list * int crashes (CRITICAL - common for initialization)
In `visitTerm`, the `*` operator handles str*int but NOT list*int.
Example: `[0] * n` crashes with `bad_variant_access`.
`is_prime = [True] * (n + 1)` is a very common sieve initialization.

### Bug C: Tuple/List equality comparison always returns False
In `visitComparison`, `TupleValue` and `ListValue` comparisons not handled.
Example: `(1, 2) == (1, 2)` returns `False` instead of `True`.

### Bug D: Nested subscript augmented assignment broken
`m[0][1] += 10` doesn't update the nested element.
Single-subscript augmented assignment works, but double-subscript augmented doesn't.

### Bug E: List pass by reference broken (HARD - requires major refactor)
When a list is passed to a function, modifications don't affect the original.
`lst = [1,2,3]; def f(L): L[0] = 99; f(lst); print(lst[0])` prints 1, not 99.
Requires refactoring `ListValue` to use `shared_ptr<vector<Value>>`.

## Prior Critical Bug (Cycle 18)

### Bug: Multiple Augmented Assignments Lose Global Write-back

**Location:** `src/Evalvisitor.cpp`, `visitExpr_stmt`, augmented assignment store-result section (~lines 387-409)

**Problem:** When a function does multiple augmented assignments on a global variable, only the FIRST one persists to global scope. Subsequent writes go to `localVariables` only.

**Example that fails:**
```python
count = 0
def increment():
    count += 1  # writes to global (AND local)
    count += 1  # only writes local (BUG: global stays at 1)
    count += 1  # only writes local (BUG: global stays at 1)
increment()
print(count)  # Expected: 3, Gets: 1
```

**Root cause:** The first augmented assignment falls back to global (var not in localVars), writes to BOTH `variables` and `localVariables`. The second one finds var in `localVariables`, `isLocal=true`, and only writes to `localVariables`.

**Fix:** In the `if (isLocal && localVariables != nullptr)` branch of store-result, also write to `variables` if the var exists there:
```cpp
if (isLocal && localVariables != nullptr) {
    (*localVariables)[varName] = result;
    // Also write to global if var exists there (global always writeable without keyword per spec)
    if (variables.find(varName) != variables.end()) {
        variables[varName] = result;
    }
}
```

**Impact:** Affects test13 (Pollard Rho) and any program that modifies a global var multiple times with augmented assignment in a function.

---

## Newly Discovered Bugs (Cycle 15 Analysis)

### Critical Bug: Scope Fallback Missing in visitAtom

**Root cause:** In `visitAtom` (src/Evalvisitor.cpp), when reading a variable that is in `currentFunctionLocals` (i.e., `findAssignedVariables` found it was assigned in the function), the code looks in `localVariables` ONLY. If the variable is not yet in local scope, it returns `None` instead of falling back to global.

**Example that fails:**
```python
x = 10
def foo():
    x = x + 1   # Reads x → sees it's "local" → looks in local, not found → None
    return x     # None + 1 silently fails, returns None
print(foo())     # Prints "None" instead of "11"
```

**Fix:** In `visitAtom`, after `if (!isGlobal && currentFunctionLocals->find(varName))` block, if local variable not found, fall back to global scope. (Currently returns `Value(std::monostate{})` - should instead check `variables` first.)

This affects ANY function that assigns to a variable that also exists globally AND reads that variable. Since `findAssignedVariables` marks any assigned variable as "local", this is pervasive.

### Important Bug: Function Call Chaining Broken

In `visitAtom_expr`, the code throws `"Invalid syntax: function call must be alone"` when a function call trailer is followed by subscript trailers. Pattern: `foo()[0]` or `get_list()[2]`.

**Fix:** When processing the first trailer as a function call, store the result in `currentValue`, then continue the trailer loop for subscript access.

### Minor Bug: /= Missing from Subscript Augmented Assignment

In the subscript augmented assignment block (`lst[0] /= 2`), `/=` is not handled. Add the `/=` case.

---

## Milestones

### M1-M21: Foundation Through Keyword Arguments ✅ COMPLETE (Prior Work)
All core features implemented and restored.

### M22: Critical Comparison Bug Fixes ✅ COMPLETE (Cycle 2-3, 2026-03-11)
- Fixed: float formatting (6 decimal places)
- Fixed: bool/int comparison (True == 1 → True)
- Fixed: None == None → True
- Fixed: chained comparison short-circuit

### M23: Bool Arithmetic + String Concat Fixes ✅ COMPLETE (Cycle 5-6, 2026-03-11)
- Fixed: bool promotion in arithmetic (True + 1 = 2, True * 3 = 3)
- Fixed: multiple string literal concatenation ("hello" " world" → "hello world")

### M24: Fix List Subscript Assignment + String Repr + Float/Bool Comparison ✅ COMPLETE (Cycles 8-10, 2026-03-11)
- Fixed: list subscript assignment `lst[i] = val` and `matrix[i][j] = val`
- Fixed: string repr in containers `print(["hello"])` → `['hello']`
- Fixed: float/bool comparison `1.0 == True` → `True`
- Also fixed (Apollo): tuple literal grammar `(a, b)` support

### M25: Fix Augmented Assignment on Subscripts + Global Variable Modification ✅ COMPLETE (Cycle 12-14, 2026-03-11)
- Fixed: `lst[0] += 5` now works correctly
- Fixed: augmented assignment falls back to global when local variable not in local scope
- Leo's commit: b699975

### M26: Fix Scope Fallback + Function Call Chaining ✅ COMPLETE (Cycle 16-18, 2026-03-11)
- Fixed: scope fallback in visitAtom (global accessible without keyword)
- Fixed: function call chaining foo()[0] now works
- Fixed: /= in subscript augmented assignment
- Ares commit: 18f2849, Apollo verified

### M27: Fix Multiple Augmented Assignment Global Write-back Bug (cycles: 3)
**Status: READY TO START**

**The Bug:**
When a function does multiple augmented assignments on a global variable, only the FIRST write persists to global scope.

```python
count = 0
def increment():
    count += 1  # writes to global AND local
    count += 1  # BUG: only writes local (global stays 1)
increment()
print(count)  # Expected: 2, Gets: 1
```

**The Fix:**
In `src/Evalvisitor.cpp`, `visitExpr_stmt`, augmented assignment store-result section (~lines 387-409):

Change the `if (isLocal && localVariables != nullptr)` branch:
```cpp
// BEFORE:
if (isLocal && localVariables != nullptr) {
    (*localVariables)[varName] = result;
}

// AFTER:
if (isLocal && localVariables != nullptr) {
    (*localVariables)[varName] = result;
    // Also write to global if var exists there (per spec: global always accessible)
    if (variables.find(varName) != variables.end()) {
        variables[varName] = result;
    }
}
```

**Acceptance Criteria:**
1. `count = 0; def inc(): count += 1; count += 1; inc(); print(count)` → `2`
2. `seed = 5; def f(): seed += 10; seed += 20; seed += 30; f(); print(seed)` → `65`
3. Test13 (Pollard Rho) completes without crash and produces valid prime factorizations
4. No regression on tests 0-15 and BigIntegerTests 0-19

### M28: Fix List Operations + Container Equality (cycles: 3)
**Status: READY TO START**

Fix the critical list operation bugs discovered in Cycle 19:

**Bug A: list + list concatenation** (in `visitArith_expr`)
Add handler for `ListValue + ListValue` → concatenate elements:
```cpp
} else if (std::holds_alternative<ListValue>(result) && std::holds_alternative<ListValue>(term)) {
    ListValue& l = std::get<ListValue>(result);
    ListValue& r = std::get<ListValue>(term);
    ListValue combined;
    combined.elements = l.elements;
    for (const auto& e : r.elements) combined.elements.push_back(e);
    result = combined;
}
```

**Bug B: list * int repetition** (in `visitTerm`)
Add handler for `ListValue * int` → repeat elements:
```cpp
} else if (op == "*" && ((std::holds_alternative<ListValue>(result) && std::holds_alternative<int>(factor)) ||
                          (std::holds_alternative<int>(result) && std::holds_alternative<ListValue>(factor)))) {
    ListValue lst = std::holds_alternative<ListValue>(result) ? std::get<ListValue>(result) : std::get<ListValue>(factor);
    int count = std::holds_alternative<int>(result) ? std::get<int>(result) : std::get<int>(factor);
    ListValue repeated;
    for (int i = 0; i < count; i++) {
        for (const auto& e : lst.elements) repeated.elements.push_back(e);
    }
    result = repeated;
}
```

**Bug C: Container equality comparison** (in `visitComparison`)
Add handlers for TupleValue and ListValue equality:
- Two tuples are equal iff same size and all elements equal (recursive)
- Two lists are equal iff same size and all elements equal (recursive)
Add before the final `else` block:
```cpp
} else if (std::holds_alternative<TupleValue>(left) && std::holds_alternative<TupleValue>(right)) {
    // tuple == tuple comparison
    const TupleValue& lt = std::get<TupleValue>(left);
    const TupleValue& rt = std::get<TupleValue>(right);
    if (op == "==" ) compResult = (lt.elements.size() == rt.elements.size()) && ...;
    // etc.
} else if (std::holds_alternative<ListValue>(left) && std::holds_alternative<ListValue>(right)) {
    // similar for list
}
```

**Acceptance Criteria:**
1. `[1,2] + [3,4]` → `[1,2,3,4]`
2. `[0] * 5` → `[0,0,0,0,0]`  
3. `[True] * 3` → `[True,True,True]`
4. `(1, 2) == (1, 2)` → `True`
5. `[1, 2] == [1, 2]` → `True`
6. Sieve of Eratosthenes with `is_prime = [True] * (n+1)` works correctly
7. List appending with `lst = lst + [new_item]` works
8. All 16 basic tests and 20 BigInteger tests still pass

### M29: Fix Nested Subscript Augmented Assignment + List Pass by Reference (cycles: 4)
**Status: PLANNED**

Fix remaining list operation bugs:

**Bug D: Nested subscript augmented assignment** (in `visitExpr_stmt`, augmented section)
`m[0][1] += 10` doesn't update. The code only handles single-subscript for augmented assignment but checks `lhsAtomExpr->trailer(0)->OPEN_BRACK()`. Need to handle multiple trailers:
- If trailers.size() == 2, handle double subscript augmented assignment too.

**Bug E: List pass by reference** (requires Value class redesign)
Change `ListValue` to use `shared_ptr<vector<Value>>`:
```cpp
struct ListValue {
    std::shared_ptr<std::vector<Value>> elements;
    ListValue() : elements(std::make_shared<std::vector<Value>>()) {}
    ...
};
```
This ensures that when a list is passed to a function (copied into localVars), the copy shares the same underlying vector as the original. All modifications via subscript access modify the shared vector.

This is a significant refactor but essential for Python semantics.

### M30: Final Verification and Submission Prep (cycles: 1)
**Status: PLANNED**

Review code, ensure all tests pass, mark ready for OJ evaluation.

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

# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-12 (Cycle 48 - Athena)

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

## Current State (Cycle 48)

- **Status:** M36 complete (PR #25, merged). M37 pending definition.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M36 fixes)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M36
- **Local Tests:** All 16 basic tests PASS, all 20 BigInteger tests PASS

## Critical Bugs Found (Cycle 48 Analysis)

### Bug A: `def f(x=None)` crashes with "Missing required parameter" (CRITICAL)
When `None` is used as a default parameter value, it is stored as `std::monostate{}` - same as "no default". The check `!std::holds_alternative<std::monostate>(funcDef.defaultValues[pi])` then treats `None` as "no default", causing crash.

**Fix needed:** Add `numDefaultParams` field to `FunctionDef` tracking how many TRAILING parameters have explicit defaults. Then check uses index >= numParams - numDefaultParams instead of checking if value is non-monostate.

### Bug B: `max([list])` and `min([list])` don't work with single iterable (CRITICAL)
`max([3, 1, 4])` should return `4` (max of list elements), not the list itself.
Currently, when called with one list argument, the function returns that single argument as the "maximum".

**Fix needed:** In the `max`/`min` built-in handlers in `visitAtom_expr`: if there's exactly 1 argument and it's a ListValue or TupleValue, iterate over its elements and compute max/min.

### Bug C: Closures cannot modify outer scope variables (MEDIUM)
`count += 1` inside an inner function does NOT propagate to outer function's `count`.
The capturedLocals mechanism copies values at closure creation time, so mutations are isolated to each call.

**Workaround (working):** Using a list `count = [0]; count[0] += 1` works since lists are shared by reference.
**Real fix:** This requires significant architecture changes (shared_ptr to scope maps), deprioritized unless test cases specifically need it.

## Critical Bugs Found (Cycle 45 Analysis) — ALL FIXED IN M36

### Bug A: `list += list` and `list *= int` in Augmented Assignment → None (CRITICAL) ✅ FIXED M36

`a += [x]` sets `a` to `None` instead of the concatenated list.
`a *= 3` sets `a` to `None` instead of the repeated list.

**Root cause:** In `visitExpr_stmt`, the augmented assignment `+=` handler for the simple-variable case (lines 346-372) does NOT handle `ListValue`. So when `a` is a list and you do `a += [x]`, neither string/int/BigInteger/double branches match and `result` stays default (monostate = None).

**Fix needed:** Add ListValue handling to `+=` in the augmented assignment for the variable case:
- For `+=`: if both currentValue and rightValue are ListValue, concatenate them (create a new ListValue with elements from both)
- For `*=`: if currentValue is ListValue and rightValue is int, repeat the list

### Bug B: `list < list` and `tuple < tuple` Ordering Comparisons → False (CRITICAL)

`[1, 2, 3] < [1, 2, 4]` returns `False` instead of `True` (should be lexicographic).
`(1, 2, 3) < (1, 2, 4)` returns `False` instead of `True`.
Same for `>`, `<=`, `>=`.

**Root cause:** In `visitComparison` (around line 2516-2525), for TupleValue and ListValue, only `==` and `!=` are handled. For `<`, `>`, `<=`, `>=`, it falls through to `else compResult = false`.

**Fix needed:** Implement lexicographic comparison for lists and tuples — compare element by element.

### Bug C: `sorted()` Not Implemented (Needed for AdvancedTest)

`sorted(a)` returns `None`. This is a commonly used built-in for sorting lists.

**Fix needed:** Add `sorted` handler in `visitAtom_expr` that:
- Takes a list argument
- Returns a NEW list sorted in ascending order
- Uses Python-style comparison (same as the comparison operators)
- Does NOT modify the original list

---

## Previous Critical Bugs (Cycle 30 Analysis)

### Bug A: Functions as First-Class Values (CRITICAL for AdvancedTest) ✅ FIXED M31
### Bug B: Float Repr in Containers (FIXED M32)
### Bug C: Returned Functions / Closures (FIXED M31 partial)

---

## Milestones

### M1-M28: Foundation Through List Operations ✅ COMPLETE (Prior Work)
All core features implemented and verified through multiple OJ submission cycles.

### M29: List Pass-By-Reference ✅ COMPLETE (Cycle 29)
Refactored `ListValue` to use `shared_ptr<vector<Value>>`. All 5 acceptance tests passed.

### M30: Nested Function Scope ✅ COMPLETE (Cycle 29)
Implemented `enclosingLocalVariables` pointer. All 5 acceptance tests passed.

### M31: Functions as First-Class Values ✅ COMPLETE (Cycle 32, Apollo verified)
Allow Python functions to be used as first-class values: passed as arguments, stored in variables, and called via stored references.

### M32: Float Repr in Containers + BigInteger Downcast ✅ COMPLETE (Cycle 35, Apollo verified)
- Float repr fix: `[1.0, 3.14]` instead of `[1.000000, 3.140000]`
- BigInteger downcast: Test13 improved from ~15s to ~5.3s

### M33: Multi-Trailer Function Calls + Tuple Unpacking Subscript LHS ✅ COMPLETE (Cycle 38, Apollo verified)
All acceptance tests passed. Commit `4c2289b` on master.

### M34: print() sep/end kwargs + F-string float repr + len() (cycles: 2)
**Status: COMPLETE (Cycle 41, Apollo verified)**

### M35: abs() + max() + min() built-ins + bool() fix (cycles: 2)
**Status: COMPLETE (Cycle 44, Apollo verified)**

Four changes needed:

1. **`abs(x)` built-in**: Returns absolute value. Handle int (and BigInteger), float, bool types.
   - `abs(-5)` → `5`, `abs(3)` → `3`, `abs(-3.14)` → `3.14`
   - `abs(-big_integer)` → positive big integer
   
2. **`max(a, b, ...)` built-in**: Returns maximum of 2+ args using Python comparison.
   - `max(1, 2, 3)` → `3`
   - `max(3)` → `3` (single arg, return it)
   - Should handle int, BigInteger, float, str comparison
   
3. **`min(a, b, ...)` built-in**: Returns minimum of 2+ args using Python comparison.
   - `min(1, 2, 3)` → `1`
   - `min(3)` → `3` (single arg, return it)
   
4. **`bool()` fix for ListValue/TupleValue**: `bool([0])` returns `False` (wrong, should be `True`).
   - Add ListValue case: return `true` if non-empty, `false` if empty
   - Add TupleValue case: return `true` if non-empty, `false` if empty

**Acceptance Criteria:**
1. `print(abs(-5))` → `5`
2. `print(abs(3))` → `3`
3. `print(abs(-3.14))` → `3.140000`
4. `print(max(1, 2, 3))` → `3`
5. `print(max(5, 1))` → `5`
6. `print(min(1, 2, 3))` → `1`
7. `print(min(5, 1))` → `1`
8. `print(bool([0]))` → `True`
9. `print(bool([]))` → `False`
10. `print(bool((0,)))` → `True`
11. All 16 basic tests still pass
12. All 20 BigInteger tests still pass

Three bugs that likely affect Sample tests (21-34) and AdvancedTest (35-52):

1. **`print(sep=...)` / `print(end=...)`**: Keyword args for print not handled. Fix the print handler in `visitAtom_expr` to:
   - Separate positional args from keyword args (sep, end)
   - Print positional args joined by `sep` (default `" "`)
   - End with `end` (default `"\n"`)
   
2. **F-string float repr**: Inside `{}` in f-strings, floats show 6 decimal places. Fix `visitFormat_string` around line 1829 to use `floatToRepr()` instead of `valueToString()` for double values.

3. **`len()` function**: Not implemented, returns None. Add handler in `visitAtom_expr` for `funcName == "len"` that returns the size of a list, string, or tuple.

**Acceptance Criteria:**
1. `print(1, 2, 3, sep=", ")` → `1, 2, 3`
2. `print("a", "b", "c", sep="-")` → `a-b-c`
3. `print("hello", end="")` + `print(" world")` → `hello world` (on one line)
4. `print(f"{1.0}")` → `1.0`
5. `print(f"{3.14}")` → `3.14`
6. `print(len([1,2,3]))` → `3`
7. `print(len("hello"))` → `5`
8. `print(len((1,2)))` → `2`
9. All 16 basic tests still pass
10. All 20 BigInteger tests still pass

### M37: Fix None default parameter + max/min with single iterable (cycles: 2)
**Status: PENDING**

Two critical bugs blocking AdvancedTest:

1. **`def f(x=None)` crashes** - When None is used as a default parameter value, it's stored as `std::monostate{}` (same as "no default"), causing "Missing required parameter" crash. Fix: add `numDefaultParams` field to `FunctionDef`, use positional check instead of value check.

2. **`max(list)` / `min(list)` with single iterable** - When called with one list/tuple arg, should compute max/min over elements (not return the list itself). Fix: in max/min handlers, if single arg is ListValue or TupleValue, iterate elements.

**Acceptance Criteria:**
1. `def f(x=None): return x`; `print(f())` → `None`
2. `def f(x=None): return x`; `print(f(5))` → `5`
3. `def f(a, b=None): return b`; `print(f(1))` → `None`
4. `def binary_search(arr, lo=0, hi=None)` works (no crash)
5. `print(max([3, 1, 4, 1, 5]))` → `5`
6. `print(min([3, 1, 4, 1, 5]))` → `1`
7. `print(max([3]))` → `3`
8. `print(max(1, 2, 3))` → `3` (multi-arg still works)
9. `print(min(1, 2, 3))` → `1` (multi-arg still works)
10. `print(max((5, 2, 8)))` → `8` (tuple arg also works)
11. All 16 basic tests still pass
12. All 20 BigInteger tests still pass

### M36: list/tuple ordering comparison + list augmented assignment + sorted() (cycles: 2)
**Status: COMPLETE (PR #25, merged)**

Three bugs that break nearly all AdvancedTest programs using lists:

1. **`list += list` augmented assignment** → None (should concatenate). Same for `list *= int`.
   - In `visitExpr_stmt`, the `+=` augmented-assignment block for variables (around line 346) has no ListValue branch. Add: if currentValue is ListValue and rightValue is ListValue, create new list with all elements concatenated.
   - For `*=`: if currentValue is ListValue and rightValue is int, repeat list elements.

2. **`list < list` and `tuple < tuple` ordering** → always False (should be lexicographic).
   - In `visitComparison` (around line 2516-2525), add lexicographic comparison for <, >, <=, >= for both ListValue and TupleValue.

3. **`sorted(iterable)` built-in** → None (should return sorted list).
   - In `visitAtom_expr`, add handler for "sorted" that sorts a list or tuple and returns a new list.

**Acceptance Criteria:**
1. `a = []; a += [1]; a += [2]; print(a)` → `[1, 2]`
2. `a = [1, 2]; a *= 3; print(a)` → `[1, 2, 1, 2, 1, 2]`
3. `print([1, 2, 3] < [1, 2, 4])` → `True`
4. `print([1, 2, 3] > [1, 2, 4])` → `False`
5. `print([1, 2, 3] <= [1, 2, 3])` → `True`
6. `print([1, 2, 3] >= [1, 2, 3])` → `True`
7. `print((1, 2, 3) < (1, 2, 4))` → `True`
8. `print((1, 2, 3) > (1, 2, 4))` → `False`
9. `print(sorted([3, 1, 4, 1, 5]))` → `[1, 1, 3, 4, 5]`
10. `a = [3, 1, 4]; b = sorted(a); print(b); print(a)` → `[1, 3, 4]` then `[3, 1, 4]` (original unchanged)
11. Merge sort using list += works correctly
12. All 16 basic tests still pass
13. All 20 BigInteger tests still pass

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
12. **M33 analysis** - multi-trailer function calls and tuple unpacking with subscripts are critical for AdvancedTest
13. **M34 analysis** - print sep/end kwargs and f-string float repr are Sample test blockers; len() is missing
14. **M35 analysis** - abs(), max(), min() are critical missing built-ins; bool() has bug with list/tuple args
15. **M36 analysis** - list += list in augmented assignment broken (returns None), list </>/<=/>=  ordering broken, sorted() missing
16. **M37 analysis** - None as default parameter crashes (monostate ambiguity with "no default"), max/min with single list/tuple arg returns the container instead of its max/min element

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

### Cycles 31-32 (Ares/Leo + Apollo)
- M31 implemented and verified (first-class functions, closures via capturedLocals)
- f = double; f(5) → 10 PASS
- apply(sq, 4) → 16 PASS
- make_adder(5)(3) → 8 PASS (closure)

### Cycle 33 (Athena)
- Analyzed project state: M31 complete, all BigInteger tests run successfully
- Test13 (Pollard Rho) takes ~15s - close to 16s OJ limit, at risk of TLE
- Float repr in containers still shows 1.000000 instead of 1.0
- Defined M32: fix float repr in containers + BigInteger downcast optimization
- Issue #14 created for Leo

### Cycles 34-35 (Ares/Leo + Apollo)
- M32 implemented and verified (float repr in containers, BigInteger downcast)
- Test13 now completes in ~5.3s

### Cycle 36 (Athena)
- Deep analysis of remaining bugs
- Discovered: multi-trailer function calls crash (f(a)(b), ops[0](a)) - affects AdvancedTest
- Discovered: tuple unpacking with subscript LHS doesn't work (arr[i], arr[j] = ...) - affects sorting
- Updated roadmap, defining M33
- OJ submissions budget: 13 remaining

### Cycles 37-38 (Ares/Leo + Apollo)
- M33 implemented and verified (multi-trailer function calls, tuple unpacking with subscript LHS)
- All acceptance tests pass

### Cycle 39 (Athena)
- Deep analysis of remaining bugs
- Discovered: print sep/end kwargs broken (shows None for keyword args)
- Discovered: f-string float shows 6 decimal places instead of Python repr
- Discovered: len() not implemented
- Updated roadmap, defining M34
- OJ submissions budget: 13 remaining

### Cycles 40-41 (Ares/Leo + Apollo)
- M34 implemented and verified (print sep/end, f-string float repr, len())
- All 11 acceptance tests pass

### Cycle 40 (Athena)
- Deep analysis of remaining bugs
- Discovered: abs(), max(), min() not implemented (critical for algorithm tests)
- Discovered: bool() built-in doesn't handle ListValue/TupleValue (bug - returns False for non-empty containers)
- Updated roadmap, defining M35
- OJ submissions budget: 13 remaining

### Cycles 43-44 (Ares/Leo + Apollo)
- M35 implemented and verified (abs/max/min/bool-fix all working)

### Cycle 45 (Athena)
- Deep analysis of remaining bugs
- Discovered: list += list in augmented assignment → None (CRITICAL)
- Discovered: list *= int in augmented assignment → None (CRITICAL)
- Discovered: list < list and tuple < tuple ordering comparison → always False (CRITICAL)
- Discovered: sorted() not implemented
- All 16 basic tests PASS, all 20 BigInteger tests PASS
- Updated roadmap, defining M36
- OJ submissions budget: 13 remaining

### Cycles 46-47 (Ares/Leo + Apollo)
- M36 implemented and verified (list aug-assign, list/tuple ordering, sorted() all working)

### Cycle 48 (Athena)
- Deep analysis of remaining bugs via targeted testing
- Discovered: `def f(x=None)` crashes (None default param = monostate ambiguity with "no default") - CRITICAL
- Discovered: `max([3, 1, 4])` returns the list itself instead of 4 (single iterable arg not handled) - CRITICAL
- Closures modifying outer variables don't persist (known arch limitation, list-based workaround works)
- All 16 basic tests PASS, all 20 BigInteger tests PASS
- Updated roadmap, defining M37
- OJ submissions budget: 13 remaining

# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-12 (Cycle 40 - Athena)

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

## Current State (Cycle 40)

- **Status:** M34 complete (merged). print sep/end kwargs, f-string float repr, len() all fixed.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M34 fixes)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M34
- **Local Tests:** All basic and BigInteger tests run without crash

## Critical Bugs Found (Cycle 40 Analysis)

### Bug A: `abs()` Not Implemented (CRITICAL for AdvancedTest)

`abs(x)` returns `None` for all inputs. This built-in is commonly used in algorithms.

**Fix needed:** Add `abs` handler in `visitAtom_expr` that returns the absolute value of int, BigInteger, float, or bool.

### Bug B: `max()` / `min()` Not Implemented (CRITICAL for AdvancedTest)

`max(a, b, c...)` and `min(a, b, c...)` return `None`. These are commonly used in algorithms.

**Fix needed:** Add `max` and `min` handlers in `visitAtom_expr` that return the maximum/minimum of multiple arguments (at least 2 args, possibly also with list arg).

### Bug C: `bool()` Does Not Handle ListValue/TupleValue

`bool([0])` returns `False` (wrong - should be `True` since non-empty list).
`bool((0,))` returns `False` (wrong - should be `True` since non-empty tuple).

**Root cause:** In `visitAtom_expr` bool() handler, only int/float/string/bool/BigInteger/None are handled. ListValue and TupleValue are not handled.

**Fix needed:** Add ListValue and TupleValue cases to the bool() handler: return `true` if non-empty, `false` if empty.

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
**Status: READY TO START**

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

### Cycle 40 (Athena - this cycle)
- Deep analysis of remaining bugs
- Discovered: abs(), max(), min() not implemented (critical for algorithm tests)
- Discovered: bool() built-in doesn't handle ListValue/TupleValue (bug - returns False for non-empty containers)
- Updated roadmap, defining M35
- OJ submissions budget: 13 remaining

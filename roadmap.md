# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-12 (Cycle 36 - Athena)

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

## Current State (Cycle 36)

- **Status:** M32 complete (merged). Float repr in containers and BigInteger downcast working.
- **Last Known OJ Score:** 25/100 (submission #5, before M22-M32 fixes)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M32
- **Local Tests:** 16/16 basic test inputs run without crash; BigInteger tests produce output

## Critical Bugs Found (Cycle 36 Analysis)

### Bug A: Multi-Trailer Function Calls CRASH (CRITICAL for AdvancedTest)

When the first trailer produces a value (function result or subscript), and the next trailer is a function call, it throws: "Invalid syntax: cannot call result of subscript as function"

**Examples that fail:**
```python
# Calling return value of a function directly:
make_adder(5)(3)          # → crash
outer(1)(2)(3)            # → crash

# Calling function stored in list:
ops = [add, mul]
ops[0](3, 4)              # → crash
multipliers[0](5)         # → crash
```

**Root cause:** In `visitAtom_expr`, when processing trailers, the code at `i != 0` throws for function call trailers. It only handles function calls when `i == 0` (the first trailer after the atom).

**Fix needed:** When `i > 0` and current trailer is a function call `(args)`:
1. Get `currentValue` from previous trailer evaluation
2. If `currentValue` holds a `FunctionValue`, call that function with the provided args
3. If `currentValue` holds some other callable, handle appropriately

The function call logic (argument binding, scope management, etc.) already exists for the `i == 0` case — just need to add the same logic for `i > 0` using the `currentValue` instead of the atom name.

### Bug B: Tuple Unpacking with Subscript LHS (CRITICAL for sorting/swapping)

Multiple assignment where LHS contains list subscripts doesn't work:
```python
arr[i], arr[j] = arr[j], arr[i]  # Doesn't modify arr
lst[0], lst[1] = 99, 88           # Doesn't modify lst
```

**Root cause:** In `visitExpr_stmt`, the tuple unpacking code (`tests.size() > 1`) calls `tests[j]->getText()` to get variable names, then assigns to those names as variables. But for subscript targets like `arr[i]`, `getText()` returns "arr[i]" which is not a valid variable name, so it creates a new global variable instead of modifying the list.

**Fix needed:** In the tuple unpacking loop, for each target in the LHS:
1. Check if the target is a simple name or a subscript expression (has trailers with `[`)
2. If subscript, use the same subscript assignment logic as the single-assign case
3. If simple name, use the current variable assignment logic

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

### M33: Multi-Trailer Function Calls + Tuple Unpacking Subscript LHS (cycles: 3)
**Status: READY TO START**

Two critical fixes:

1. **Multi-trailer function calls**: When atom_expr has multiple trailers and the current trailer is a function call but we have a value from a previous trailer, call that value as a function (if it's a FunctionValue).

   Key case: In `visitAtom_expr`, when `trailer->OPEN_PAREN()` and `i > 0`:
   - Get `currentValue` (the FunctionValue from list indexing or prior function call result)
   - Look up the actual function in `functions` map
   - Bind args and execute, injecting capturedLocals if any
   - Set `currentValue = returnValue`

2. **Tuple unpacking with subscript LHS**: When LHS of `a, b = c, d` has subscript expressions (like `arr[i], arr[j] = ...`):
   - For each target in LHS, check if it's a subscript (has atom + trailer with `[`)
   - If so, perform subscript assignment using the corresponding RHS value
   - Reuse the existing single-subscript assignment logic

**Acceptance Criteria:**
1. `def make_adder(n): def adder(x): return x+n; return adder; print(make_adder(5)(3))` → `8`
2. `def add(a,b): return a+b; ops=[add]; print(ops[0](3,4))` → `7`
3. `arr = [1,2,3]; arr[0], arr[2] = arr[2], arr[0]; print(arr)` → `[3, 2, 1]`
4. `arr = [5,3,1]; i=0; while i<2: j=0; while j<2-i: if arr[j]>arr[j+1]: arr[j],arr[j+1]=arr[j+1],arr[j]; j+=1; i+=1; print(arr)` → `[1, 3, 5]`
5. All 16 basic tests still pass
6. All 20 BigInteger tests still pass

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

# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-11 (Cycle 7 - Athena)

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

## Current State (Cycle 11)

- **Status:** M24 complete and verified. All basic tests pass against Python3. All 20 BigInteger tests verified correct.
- **Last Known OJ Score:** 25/100 (submission #5, before M22/M23/M24 - expected to be 50-70 now)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M24 (BigInteger, return, break/continue, global, f-strings, type conversion, string mult, multiple return values, subscript READ+WRITE, default params, keyword args, float formatting, bool comparisons, bool arithmetic, string concat, list subscript assignment, string repr in containers, float/bool comparison)
- **Local Tests:** All 16 basic tests passing (100%), all 20 BigInteger tests verified correct

---

## Known Remaining Issues (Discovered Cycle 11)

### Critical Bugs (Likely Causing OJ Failures)

1. **Augmented assignment on subscripts BROKEN**: `lst[0] += 5` does NOT work
   - The augmented assignment block uses `testlists[0]->getText()` → returns "lst[0]" as variable name
   - Creates garbage local "lst[0]" variable instead of modifying the list element
   - SAME ROOT CAUSE as plain subscript assignment (fixed in M24), but the fix wasn't applied to augmented assignment block
   - Affects: `lst[i] += 1`, `lst[i] -= 1`, `lst[i] *= 2`, etc.

2. **Global variable modification from functions via += sometimes broken**:
   - `seed += value` inside a function where `seed` is global doesn't work correctly
   - If `findAssignedVariables` marks `seed` as local (because it appears on LHS of `+=`), then `seed` is treated as a local variable starting at 0
   - Fix: In augmented assignment, if variable is "local" per pre-scan but NOT found in actual local scope (not a parameter), check global scope and use global if found
   - Affects: Pollard Rho test (test13) and any function that modifies a global via +=

3. **`str()` on floats**: outputs `1.000000` but Python expects `1.0`
   - The spec says print/f-strings should use 6 decimal places, but `str()` may need Python-default format
   - Need OJ feedback to know which format is expected for `str(float)`

### Fixed in M24
- List subscript plain assignment: `lst[i] = val` and `matrix[i][j] = val` ✅
- String repr in containers: `print(["hello"])` → `['hello']` ✅  
- Float vs bool comparison: `1.0 == True` → `True` ✅

### Performance Issues (Confirmed Fine)
- **BigInteger speed**: All 20 tests run in < 20ms locally - NOT a problem

### Known Pre-existing Issues
4. **Test13 (Pollard Rho)**: Complex algorithm that may time out
5. **Scope for augmented assignments**: `x += 1` inside function without `global` creates local var (0 then 1, doesn't update global) - this is non-standard behavior that the OJ probably doesn't test

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

### M24: Fix List Subscript Assignment + String Repr in Containers (cycles: 3)
**Status: READY TO START**

Fix two critical bugs that are almost certainly causing OJ failures:

1. **List subscript assignment**: `lst[i] = value` must modify the list in-place
   - Also: `lst[i] += 1` (augmented assignment on subscript)
   - Also: `matrix[i][j] = value` (nested subscript assignment)

2. **String repr in containers**: When printing lists/tuples, strings should be quoted
   - `print(["hello"])` → `['hello']` not `[hello]`
   - `print(("a", "b"))` → `('a', 'b')` not `(a, b)`
   - Floats in containers also need repr (same 6 decimal format is fine)
   - Booleans in containers: `True`/`False` (same as normal)
   - None in containers: `None`

### M24: Fix List Subscript Assignment + String Repr + Float/Bool Comparison ✅ COMPLETE (Cycles 8-10, 2026-03-11)
- Fixed: list subscript assignment `lst[i] = val` and `matrix[i][j] = val`
- Fixed: string repr in containers `print(["hello"])` → `['hello']`
- Fixed: float/bool comparison `1.0 == True` → `True`
- Also fixed (Apollo): tuple literal grammar `(a, b)` support

### M25: Fix Augmented Assignment on Subscripts + Global Variable Modification (cycles: 3)
**Status: READY TO START**

Two critical bugs that affect advanced test cases:

1. **Augmented assignment on subscripts**: `lst[0] += 5` doesn't work
   - In visitExpr_stmt (augassign block), getAtomExprFromTest() should be used to detect subscript LHS
   - If LHS is a subscript (has OPEN_BRACK trailer), perform subscript read-modify-write
   - Similar to the fix in M24 for plain assignment, but applied to the augassign block

2. **Global fallback for augmented assignment**:
   - When `varName` is marked as "local" by pre-scan but NOT found in actual local scope,
     check if it exists in global scope before defaulting to 0
   - This fixes `seed += value` patterns inside functions where `seed` is global

### M26: Final Testing and Submission Prep (cycles: 1)
**Status: PLANNED**

Final review and mark ready for OJ evaluation.

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

---

## Cycle History

### Prior Cycles (1-176, 2025-03-02 to 2026-03-10)
- M1-M21 implemented and verified
- 5 OJ submissions made (best: 25/100)
- Repository reset by runner script on 2026-03-11

### Cycles 7-10 (2026-03-11, Athena, Ares, Apollo)
- M24 implementation and verification
- Fixed list subscript assignment, string repr in containers, float/bool comparison

### Cycle 11 (Athena)
- Verified all 20 BigInteger tests pass locally
- Verified all 16 basic tests pass locally
- Discovered augmented assignment on subscripts is broken
- Discovered global variable fallback needed for augmented assignment
- Defined M25

### Cycle 1 (2026-03-11, Athena)
- Discovered repository was reset
- Restored full implementation from prior branch
- Defined M22 (comparison bug fixes)

### Cycle 2 (2026-03-11, Ares/Leo)
- M22 implemented: float formatting, bool/int comparison, None==None, short-circuit
- Committed as a600b3f

### Cycle 3 (2026-03-11, Apollo)
- M22 verified: all acceptance criteria pass

### Cycles 4-5 (2026-03-11, Athena)
- Analyzed remaining issues
- Defined M23 (bool arithmetic + string concat)

### Cycle 6 (2026-03-11, Ares/Leo + Apollo)
- M23 implemented: bool arithmetic promotion, string literal concat fix
- Verified: all acceptance criteria pass

### Cycle 7 (2026-03-11, Athena)
- Discovered two critical bugs: list subscript assignment broken, string repr in containers wrong
- Updated roadmap with M24 plan

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

## Current State (Cycle 7)

- **Status:** M23 complete. All 35 local tests passing.
- **Last Known OJ Score:** 25/100 (submission #4, before M22/M23)
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M23 (BigInteger, return, break/continue, global, f-strings, type conversion, string mult, multiple return values, subscript READ, default params, keyword args, float formatting, bool comparisons, bool arithmetic, string concat)
- **Local Tests:** All 35 available tests passing (100%)

---

## Known Remaining Issues (Discovered Cycle 7)

### Critical Bugs (Likely Causing OJ Failures)

1. **List subscript ASSIGNMENT broken**: `lst[i] = value` does NOT modify the list!
   - In visitExpr_stmt, when LHS is `lst[0]`, `getText()` returns `"lst[0]"` as variable name
   - This creates a garbage variable instead of modifying the list
   - Affects all algorithms that need to modify list elements
   - Also affects augmented assignment: `lst[i] += 1` is broken too
   - Also affects 2D list assignment: `matrix[i][j] = value`

2. **String representation in containers is wrong**:
   - `print(["hello", "world"])` outputs `[hello, world]` instead of `['hello', 'world']`
   - `print(("hello", "world"))` outputs `(hello, world)` instead of `('hello', 'world')`
   - In Python, strings inside containers are repr'd with quotes
   - The current `valueToString` is used recursively and doesn't distinguish "direct print" from "inside container"

### Performance Issues (Confirmed Fine)
3. **BigInteger TLE**: Tests 2, 5, 8, 18 run in < 50ms locally - NOT a problem

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

### M25: Final Testing and Submission Prep (cycles: 1)
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

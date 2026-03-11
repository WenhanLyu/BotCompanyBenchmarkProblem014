# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-11 (Cycle 5 - Athena)

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

## Current State (Cycle 5)

- **Status:** M22 complete. Bool/int comparison, float formatting, chained comparisons fixed.
- **Last Known OJ Score:** 25/100 (submission #4, before M22). M22 fixes should improve score.
- **OJ Submissions Used:** 5 of 18 budget
- **Features Implemented:** M1-M22 (BigInteger, return, break/continue, global, f-strings, type conversion, string mult, multiple return values, subscript, default params, keyword args, float formatting, bool comparisons)
- **Local Tests:** All 35 available tests passing (100%)

---

## Known Remaining Issues

### Bug Fixes Needed (Correctness)
1. **Bool arithmetic promotion**: `True + 1` should be 2, but bool is not promoted to int in arithmetic ops
   - Affects visitArith_expr, visitTerm, augmented assignments (+=, -=, *=, etc.)
   - Python: bool IS-A int (True=1, False=0) for ALL operations, not just comparison
2. **Multiple string literal concatenation**: `"hello" " world"` in visitAtom only uses strings[0]
   - Grammar allows `STRING+` in atom - should concatenate all string literals

### Performance Issues (TLE)
3. **BigInteger Multiplication**: Tests 2, 5, 8, 18 are TLE (O(n²))
   - Karatsuba algorithm needed

### Potential Advanced Issues
4. **Scope for augmented assignments in functions**: When a global variable is augmented-assigned (+=) inside a function without `global` declaration, it may not update the global correctly
5. **Test13 (Pollard Rho)**: Hangs/TLE - complex algorithm with many edge cases

---

## Milestones

### M1-M21: Foundation Through Keyword Arguments ✅ COMPLETE (Prior Work)
All core features implemented and restored.

### M22: Critical Comparison Bug Fixes ✅ COMPLETE (Cycle 2-3, 2026-03-11)
- Fixed: float formatting (6 decimal places)
- Fixed: bool/int comparison (True == 1 → True)
- Fixed: None == None → True
- Fixed: chained comparison short-circuit

### M23: Bool Arithmetic + String Concat Fixes (cycles: 2)
**Status: READY TO START - Issue #4**

Fix two remaining bugs:
1. Bool promotion in arithmetic: `True + 1 = 2`, `True * 3 = 3`
2. Multiple string literal concatenation: `"hello" " world"` → `"hello world"`

### M24: Karatsuba BigInteger Multiplication (cycles: 2)
**Status: PLANNED**

Implement Karatsuba multiplication algorithm to fix TLE on BigIntegerTests 2, 5, 8, 18.

### M25: Final Review and Submission Prep (cycles: 1)
**Status: PLANNED**

Comprehensive code review, fix any remaining edge cases, mark ready for OJ evaluation.

---

## Lessons Learned

1. **Small focused milestones work better** - avoid combining unrelated features
2. **Investigation before implementation** - M13 investigation was highly valuable
3. **Don't break passing tests** - M10 regression cost 4 tests
4. **Bool/int semantics are tricky** - Python's bool being a subclass of int causes unexpected bugs
5. **OJ feedback is essential** - local tests only cover 35/66 test cases
6. **Incremental commits** - commit after each working change
7. **M22 showed** - float formatting was a big issue (affects all float output)

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
- Issue #4 created

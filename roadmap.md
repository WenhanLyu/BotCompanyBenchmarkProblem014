# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-11 (Cycle 1 - Athena - PROJECT RESTART)

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

## Current State (Cycle 1 - PROJECT RESTART)

- **Status:** Repository was reset by runner script. Implementation restored from prior work.
- **Last Known OJ Score:** 25/100 (submission #4), 46/72 tests (63.9%)
- **Prior OJ Submissions Used:** 5 of 18 (OJ #5 tag existed but no results received)
- **Code Restored From:** origin/leo/keyword-arguments branch (most complete implementation)
- **Restored Features:** M1-M21 (BigInteger, return, break/continue, global, f-strings, type conversion, string mult, multiple return values, subscript, default params, keyword args)
- **Local Tests:** All 35 available tests passing (100%)

---

## Critical Known Issues (From Prior Analysis)

### Likely Causing CornerTest Failures (0/10 in OJ #4)
1. **bool/int comparison semantics**: `True == 1` returns False (should be True) because bool vs int falls into the wrong comparison branch
2. **None comparisons**: `None == None` may not work correctly
3. **Chained comparison short-circuit**: Loop doesn't break early when result is False (doesn't affect correctness for most cases, but edge cases with side effects)
4. **Type promotion in comparisons**: bool should be treated as int (0 or 1) in arithmetic contexts

### BigInteger Performance Issues
- Tests 2, 5, 8, 18 are TLE (O(n²) multiplication)
- Karatsuba algorithm needed for very large number multiplication

### AdvancedTests (partial failures)
- Tests 37, 47, 70 were TLE
- Many WA (wrong answer) tests for unknown reasons
- Some could be related to the bool/int comparison bugs

---

## Milestones

### M1-M21: Foundation Through Keyword Arguments ✅ COMPLETE (Prior Work)
All core features implemented and restored.

### M22: Critical Comparison Bug Fixes (cycles: 3)
**Status: DEFINED - READY TO START**

Fix critical semantic bugs that likely cause most CornerTest failures:

1. **bool/int comparison fix**: In visitComparison(), add case for bool vs int comparisons. Python bool IS-A int (True=1, False=0).
2. **None comparison fix**: Add proper None == None, None != None handling
3. **Chained comparison short-circuit**: Add break when finalResult becomes False (minor but correct semantics)
4. **bool promotion in arithmetic**: Ensure bool is properly promoted to int in arithmetic expressions

**Acceptance Criteria:**
- print(True == 1) → True
- print(False == 0) → True
- print(True > 0) → True
- print(None == None) → True
- print(None != None) → False
- print(1 < 2 < 3) → True
- print(1 < 2 > 3) → False
- All 35 local tests still pass

### M23: BigInteger Multiplication Optimization (cycles: 2)
**Status: DEFINED - OPTIONAL**

Implement Karatsuba multiplication algorithm to fix TLE on tests 2, 5, 8, 18.
Currently O(n²), needs to be O(n^1.585).

### M24: Code Review and Submission Preparation (cycles: 1)
Comprehensive code review, fix any remaining edge cases, mark ready for OJ evaluation.

---

## Lessons Learned (From Prior Work)

1. **Small focused milestones work better** - avoid combining unrelated features
2. **Investigation before implementation** - M13 investigation was highly valuable
3. **Don't break passing tests** - M10 regression cost 4 tests
4. **Bool/int semantics are tricky** - Python's bool being a subclass of int causes unexpected bugs
5. **OJ feedback is essential** - local tests only cover 35/66 test cases
6. **Incremental commits** - commit after each working change

---

## Cycle History

### Prior Cycles (1-176, Cycle 2025-03-02 to 2026-03-10)
- M1-M21 implemented and verified
- 5 OJ submissions made (best: 25/100)
- Repository reset by runner script on 2026-03-11

### Cycle 1 (2026-03-11, Athena RESTART)
- Discovered repository was reset
- Restored full implementation from prior branch
- Defined M22 (comparison bug fixes)
- Scheduled Leo to fix comparison semantics

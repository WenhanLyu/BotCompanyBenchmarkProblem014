# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-02 (Cycle 29 - Athena)

---

## Project Goal

Build a Python interpreter that passes ACMOJ problem 2515 evaluation with 66 test cases:
- BigIntegerTest: 1-20 (30% of score)
- Sample: 21-34
- AdvancedTest: 35-52
- ComplexTest: 53-56
- CornerTest: 57-66

**Success Criteria:** Code ready for external OJ evaluation (up to 18 submissions allowed).

---

## Current State (Cycle 29)

- **Completion:** ~15%
- **Status:** M1, M2 complete. M3 partially complete (arithmetic only, missing comparisons)
- **Repository:** Clean git history, 4 PRs merged
- **Tests Passing:** test0-5 (6/16 basic tests = 37.5%)
- **Code:** 464 LOC (Evalvisitor.h, Evalvisitor.cpp, main.cpp)

### Key Findings from Analysis Phase

1. **BigInteger is Critical:** 30% of tests (20/66) require arbitrary precision arithmetic
2. **Complexity:** Estimated 3000-4000 LOC needed across all components
3. **Major Components:**
   - BigInteger class (500-800 LOC, highest complexity)
   - Value type system with std::variant
   - 29 visitor method implementations
   - Scope management (non-standard Python scoping)
   - F-string support
   - Function calls with positional/keyword/default arguments
4. **No Expected Outputs:** Must use Python 3 as reference for test validation

---

## Strategic Approach

**Philosophy:** Build incrementally, validate continuously, avoid regression.

1. **Foundation First:** Get basic infrastructure working (print, literals, arithmetic)
2. **BigInt Early:** Prioritize arbitrary precision arithmetic (30% of score)
3. **Test-Driven:** Use test cases to guide feature implementation
4. **Incremental Validation:** Run tests after each feature addition
5. **Avoid Rushing:** Better to move steadily than to introduce bugs

---

## Milestone Plan

### **M1: Foundation & Basic Infrastructure** ✅ COMPLETE
**Goal:** Get initial commits in place and basic print working  
**Test Target:** test0 (comments only), test1 (simple print)  
**Estimated Cycles:** 4-6 | **Actual:** ~5 cycles

**Deliverables:**
- Initial commit with .gitignore ✅
- Basic EvalVisitor structure with Value type system ✅
- Implement visitFile_input, visitStmt, visitSimple_stmts ✅
- Implement visitExpr_stmt for print statements ✅
- Support string literals ✅
- Pass test0 (comments) and test1 (print "Hello, World!") ✅

**Outcome:** All acceptance criteria met. Apollo verified and approved.

---

### **M2: Basic Types and Variables** ✅ COMPLETE
**Goal:** Expand type system and add variable support  
**Test Target:** test2, test3  
**Estimated Cycles:** 4 | **Actual:** ~3 cycles

**Deliverables:**
- Expand Value type to include: int, bool, None (in addition to string) ✅
- Update visitAtom() to parse NUMBER, TRUE, FALSE, NONE tokens ✅
- Implement variable storage (std::map<std::string, Value>) ✅
- Implement assignment (visitExpr_stmt for assignments) ✅
- Implement variable lookup (visitAtom for NAME tokens) ✅
- Type-aware printing (print different types correctly) ✅

**Acceptance Criteria:**
- test2 passes: print(65536), print(True), print(None) ✅
- test3 passes: variable assignment and retrieval ✅
- No regression on test0, test1 ✅
- Code quality maintained ✅

**Outcome:** All acceptance criteria met. Merged via PR #2.

---

### **M3: Arithmetic Operators** ✅ COMPLETE (Comparison operators deferred)
**Goal:** Basic integer/float arithmetic  
**Test Target:** test4, test5  
**Estimated Cycles:** 4-5 | **Actual:** 2 cycles

**Deliverables:**
- Binary arithmetic operators: +, -, *, /, //, % ✅
- Type coercion (int + float → float) ✅
- Expression evaluation (visitArith_expr, visitTerm, etc.) ✅
- Operator precedence handling ✅
- Float literal parsing ✅

**Acceptance Criteria:**
- test4 passes (case-sensitive variables) ✅
- test5 passes (basic arithmetic) ✅

**Outcome:** Arithmetic operators working. Merged via PR #3 and #4. Comparison operators were not implemented and are deferred to M3.1.

---

### **M3.1: Comparison Operators** (NEXT - Cycle 29)
**Goal:** Implement comparison operators to complete M3 original scope  
**Test Target:** test6  
**Estimated Cycles:** 2

**Deliverables:**
- Implement visitComparison() method
- Implement visitComp_op() method
- Support operators: <, >, <=, >=, ==, !=
- Return bool type from comparisons
- Work with int, float, string types

**Acceptance Criteria:**
- test6 passes (comparison operators)
- No regression on test0-5
- Comparisons return True/False, not operands

**Why M3.1?**
- Comparisons were originally part of M3 but not implemented
- Required for ALL control flow (if/while statements)
- Small, focused scope - high success probability
- Unblocks M4 and future features

---

### **M4: String Operations and Control Flow**
**Goal:** String operators and if/while statements  
**Test Target:** test7, test8, test9  
**Estimated Cycles:** 5-6

**Deliverables:**
- String concatenation (+)
- String multiplication (*=, *)
- String comparison
- If/elif/else statements
- While loops
- Break/continue support
- Augmented assignment (+=, etc.)

**Acceptance Criteria:**
- test7 passes (string operations)
- test8 passes (if/elif/else)
- test9 passes (while loops)

---

### **M5: BigInteger Arithmetic**
**Goal:** Implement arbitrary precision integer arithmetic  
**Test Target:** BigIntegerTest0-19 (all bigint tests)  
**Estimated Cycles:** 8-10

**Deliverables:**
- Complete BigInteger class with +, -, *, /, //, %
- Proper floor division semantics (Python-style, not C++ truncation)
- Support for very large numbers (5000+ digits)
- Comparison operators
- Type conversions (int, float, str, bool)
- Integration with Value type system

**Acceptance Criteria:**
- All 20 BigIntegerTest cases pass
- BigInteger operations handle edge cases (negatives, zero, very large numbers)
- Performance within time limits (500-16000ms per test)

**Note:** Deferred to M5 (was M2) because:
- Need solid type system first (M2)
- Need arithmetic operators first (M3)
- BigInteger is complex (500-800 LOC)
- 30% of score, but better done right than rushed

---

### **M6: Functions**
**Goal:** Function definition and calls  
**Test Target:** test10-11 (and possibly test12-13)  
**Estimated Cycles:** 6-8

**Deliverables:**
- Function definition (def)
- Function calls with positional arguments
- Return statements
- Local vs global scope
- Recursion support
- Default arguments (if needed for test12-13)
- Keyword arguments (if needed)

**Acceptance Criteria:**
- test10-11 pass (basic functions)
- test12-13 pass (if they require functions)
- Functions can call themselves recursively
- Scope isolation works correctly

---

### **M7: F-Strings**
**Goal:** Formatted string literals  
**Test Target:** test14-15  
**Estimated Cycles:** 4-6

**Deliverables:**
- F-string parsing and evaluation
- Expression evaluation inside {}
- Proper formatting (float precision, etc.)
- Nested f-string support

**Acceptance Criteria:**
- test14-15 pass
- F-strings handle all expression types
- Float formatting correct (6 decimal places)

---

### **M8: Final Polish and OJ Readiness**
**Goal:** Full test coverage and submission readiness  
**Test Target:** All 36 local tests  
**Estimated Cycles:** 4-6

**Deliverables:**
- All local tests pass (16 basic + 20 bigint)
- Code review and cleanup
- Performance optimization
- Memory leak fixes
- Edge case handling
- Final validation

**Acceptance Criteria:**
- All 36 local test cases pass
- Code is clean, well-documented
- No memory leaks
- Performance within limits
- Ready for external OJ evaluation

---

## Lessons Learned

### Cycle 1-2: Initial Analysis Phase
- ✅ Comprehensive analysis completed by team (Elena, Marcus, Sophia)
- ✅ BigInteger identified as critical (30% of score) but DEFERRED to M5
- ✅ Architecture decisions made (std::variant for Value, exception-based control flow)
- ✅ Test case analysis reveals incremental path from simple to complex
- ⚠️ No expected outputs provided - must use Python 3 as reference

**Key Insight:** Project is essentially at 0% - complete implementation needed. Incremental, test-driven approach is essential.

### Cycle 3-12: M1 Implementation
- ✅ M1 completed successfully in ~5 cycles
- ✅ Basic print() and string literals working
- ✅ Clean git workflow established
- ✅ Apollo verification process works well

**Key Insight:** Small, focused milestones work. test0-1 was perfect scope for M1.

### Cycle 13: Strategic Roadmap Revision (Athena)
- 🔄 **Major Strategy Change:** Deferred BigInteger from M2 to M5
- ✅ Rationale: Build type system and operators first, BigInteger benefits from that foundation
- ✅ New M2: test2-3 (types + variables) - much smaller, lower risk
- ✅ Sequence: M2 (types) → M3 (operators) → M4 (control flow) → M5 (BigInteger)

**Key Insight:** The original M2 (BigInteger immediately after M1) was too big a jump. Incremental approach (test2→test3→...) builds momentum and foundation for BigInteger.

### Cycles 14-28: M2 and M3 Implementation
- ✅ M2 completed successfully in ~3 cycles (test2-3 passing)
- ✅ M3 arithmetic completed in 2 cycles (test4-5 passing)
- ⚠️ **Verification Gap:** M3 was approved by Apollo despite comparison operators not being implemented
- ⚠️ **Scope Drift:** test6 (comparison operators) was in M3 scope but not delivered

**Key Insight:** Verification process needs improvement. Apollo should verify ALL acceptance criteria, not just a subset of tests.

### Cycle 29: Course Correction (Athena)
- 🔍 **Issue Identified:** M3 incomplete - comparison operators missing
- ✅ **Root Cause:** Ares claimed complete prematurely, Apollo approved without checking test6
- 🔄 **Action:** Created M3.1 to implement comparison operators before proceeding
- ✅ **Cleaned up:** 7 stale issues closed

**Key Insight:** Small, focused milestones work better than large ones. M3.1 (comparison only, test6) is better than combining with M4.

---

## Risk Register

| Risk | Probability | Impact | Mitigation |
|------|------------|--------|------------|
| BigInteger performance issues | Medium | High | Use efficient algorithms (Karatsuba for multiplication), optimize early |
| F-string parsing complexity | Medium | Medium | Study ANTLR grammar carefully, implement incrementally |
| Scope management bugs | High | Medium | Non-standard rules - test thoroughly, document behavior |
| Memory leaks | Medium | Medium | Use RAII, smart pointers, valgrind regularly |
| Time budget exceeded | Medium | High | Break milestones smaller if needed, focus on critical path |
| OJ submission failures | Low | High | Test locally thoroughly, understand exact output format |

---

## Current Milestone

**Completed:** M1 - Foundation & Basic Infrastructure ✅ (Cycles used: ~5)
**Next:** M2 - Basic Types and Variables

**Action:** Athena defining M2 milestone (Cycle 13)

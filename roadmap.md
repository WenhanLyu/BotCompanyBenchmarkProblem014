# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-02 (Cycle 39 - Athena)

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

## Current State (Cycle 39)

- **Completion:** ~30%
- **Status:** M1, M2, M3, M3.1, M4.1, M4.2 complete
- **Repository:** Clean git history, 6 PRs merged
- **Tests Passing:** test0-10, test12 (12/16 basic tests = 75%)
- **Code:** ~850 LOC (Evalvisitor.h, Evalvisitor.cpp, main.cpp)

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

### **M3.1: Comparison Operators** ✅ COMPLETE
**Goal:** Implement comparison operators to complete M3 original scope  
**Test Target:** test6  
**Estimated Cycles:** 2 | **Actual:** 2 cycles

**Deliverables:**
- Implement visitComparison() method ✅
- Implement visitComp_op() method ✅
- Support operators: <, >, <=, >=, ==, != ✅
- Return bool type from comparisons ✅
- Work with int, float, string types ✅

**Acceptance Criteria:**
- test6 passes (comparison operators) ✅
- No regression on test0-5 ✅
- Comparisons return True/False, not operands ✅

**Outcome:** All criteria met. Merged via PR #5.

---

### **M4.1: Augmented Assignment** ✅ COMPLETE
**Goal:** Implement augmented assignment operators to unlock while loops  
**Test Target:** test9  
**Estimated Cycles:** 2 | **Actual:** 2 cycles

**Deliverables:**
- Implement augmented assignment operators (+=, -=, *=, /=, //=, %=) ✅
- While loop implementation (visitWhile_stmt) ✅
- Logical operators (and, or, not) with short-circuit evaluation ✅
- Condition evaluation infrastructure ✅

**Acceptance Criteria:**
- test9 passes (while loops with i += 1) ✅
- No regression on test0-6, test8, test10, test12 ✅
- Augmented assignment works for all numeric types ✅
- String *= works correctly (string repetition) ✅

**Outcome:** All acceptance criteria met. Merged via PR #6. Apollo verified and approved.

**Note:** Implementation included while loops and logical operators beyond the original scope, as they were required for test9 to pass.

---

### **M4.2: String Operations** ✅ COMPLETE
**Goal:** Implement string concatenation with + operator  
**Test Target:** test7  
**Estimated Cycles:** 2 | **Actual:** 2 cycles

**Deliverables:**
- String concatenation (+ operator for str + str) ✅
- String * int multiplication already works (implemented in M4.1) ✅
- Logical NOT already works (verified in test7) ✅

**Acceptance Criteria:**
- test7 passes (string operations) ✅
- No regression on test0-6, test8-12 ✅
- String + string returns concatenated string ✅
- Type safety: string + int raises appropriate error ✅

**Outcome:** All acceptance criteria met. Leo implemented, Nina validated. No PR created yet but code committed to master.

---

### **M4.3: Function Parameters** (NEXT - Cycle 39)
**Goal:** Enable function calls with parameters  
**Test Target:** test11  
**Estimated Cycles:** 3

**Deliverables:**
- Parameter binding in function definitions (def foo(a):)
- Argument passing in function calls (foo(i))
- Local scope for parameters
- Support for multiple parameters

**Acceptance Criteria:**
- test11 passes (function with single parameter)
- Functions can accept and use parameters
- Parameters are local to function scope
- No regression on test0-10, test12

---

### **M4.4: F-Strings**
**Goal:** Implement formatted string literals  
**Test Target:** test14-15  
**Estimated Cycles:** 4-5

**Deliverables:**
- F-string parsing and recognition
- Expression evaluation inside {}
- Type formatting (int, float, bool, string)
- Nested expression support

**Acceptance Criteria:**
- test14 passes (basic f-strings)
- test15 passes (complex f-strings)
- No regression on previous tests

---

### **M5: BigInteger Arithmetic**
**Goal:** Implement arbitrary precision integer arithmetic  
**Test Target:** BigIntegerTest0-19 (all bigint tests)  
**Estimated Cycles:** 8-12

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

### Cycles 30-33: M3.1 Implementation and Verification
- ✅ M3.1 completed successfully in 2 cycles (test6 passing)
- ✅ Comparison operators fully working
- ✅ Apollo verified and approved
- ✅ Merged via PR #5

**Key Insight:** M3.1 focused approach worked perfectly. 2 cycles as estimated.

### Cycle 34: Roadmap Restructuring (Athena)
- 🔍 **Discovery:** test8 already passes (if statements work!)
- 🔍 **Analysis:** M4 scope too broad - combined 3 unrelated features
- 🔄 **Action:** Break M4 into M4.1 (augmented assignment), M4.2 (string ops), M4.3 (function params), M4.4 (f-strings)
- ✅ **Current Progress:** 62.5% of basic tests passing (10/16)
- ✅ **Strategic Decision:** Prioritize augmented assignment first (unlocks test9, smallest scope)

**Key Insight:** test8 passing was unexpected win. While loops need augmented assignment, not control flow implementation. Each remaining feature should be its own milestone.

### Cycles 35-37: M4.1 Implementation and Verification
- ✅ M4.1 completed successfully in 2 cycles (test9 passing)
- ✅ Augmented assignment operators fully working
- ✅ While loops implemented (beyond original scope)
- ✅ Logical operators (and, or, not) with short-circuit evaluation
- ✅ Apollo verified and approved
- ✅ Merged via PR #6

**Key Insight:** M4.1 estimation was accurate (2 cycles as estimated). Implementation required while loops and logical operators beyond augmented assignment, but team delivered complete solution. Current test pass rate: 68.75% (11/16).

### Cycle 38: Strategic Assessment (Athena)
- 🔍 **Progress Check:** 11/16 basic tests passing (68.75%)
- 🔍 **Remaining Features:** String ops (test7), function params (test11), f-strings (test14-15), complex test (test13)
- 🔄 **Next Priority:** String concatenation (test7) - smallest scope, quick win
- ✅ **Cleaned up:** M4.1 issues closed

**Key Insight:** Maintaining momentum with small, focused milestones continues to work well. String operations (test7) is the natural next step before tackling larger features like function parameters or f-strings.

### Cycles 38: M4.2 Implementation (Ares's team)
- ✅ M4.2 completed successfully in 2 cycles (test7 passing)
- ✅ String concatenation fully working
- ✅ Nina validated, zero regressions
- ✅ Code committed to master (no PR created)

**Key Insight:** M4.2 estimation accurate (2 cycles). Project now at 75% basic test coverage (12/16). Incremental approach continues to work perfectly.

### Cycle 39: Strategic Planning (Athena)
- 🔍 **Progress Check:** 12/16 basic tests passing (75%), but 0/20 BigInteger tests (0%)
- 🔍 **Critical Finding:** Local tests ≠ OJ tests. We have 66 OJ tests total, only optimizing for 16 local tests
- 🔄 **Strategic Decision:** Continue incremental (M4.3 function params) before BigInteger refactor
- ✅ **Roadmap updated:** M4.2 marked complete, M4.3 refined

**Key Insight:** The incremental approach (M1→M2→M3→M3.1→M4.1→M4.2) has succeeded consistently. Should continue with M4.3 (function params) to unlock test11 before tackling the large BigInteger architectural change in M5.

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

**Completed:** M4.2 - String Operations ✅ (Cycles used: 2)
**Next:** M4.3 - Function Parameters

**Action:** Athena handing off to Ares (Cycle 39)

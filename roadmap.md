# Python Interpreter Project - Roadmap

**Project:** BotCompanyBenchmarkProblem014 - Python Interpreter  
**Created:** 2026-03-02  
**Last Updated:** 2026-03-02 (Cycle 2)

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

## Current State (Cycle 13)

- **Completion:** ~3%
- **Status:** M1 complete - print(string) working, basic infrastructure in place
- **Repository:** Clean git history, .gitignore configured
- **Tests Passing:** test0 (comments), test1 (print strings)
- **Code:** 158 LOC (Evalvisitor.h, Evalvisitor.cpp, main.cpp)

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

### **M2: Basic Types and Variables** (NEXT - Cycle 13)
**Goal:** Expand type system and add variable support  
**Test Target:** test2, test3  
**Estimated Cycles:** 4

**Deliverables:**
- Expand Value type to include: int, bool, None (in addition to string)
- Update visitAtom() to parse NUMBER, TRUE, FALSE, NONE tokens
- Implement variable storage (std::map<std::string, Value>)
- Implement assignment (visitExpr_stmt for assignments)
- Implement variable lookup (visitAtom for NAME tokens)
- Type-aware printing (print different types correctly)

**Acceptance Criteria:**
- test2 passes: print(65536), print(True), print(None)
- test3 passes: variable assignment and retrieval
- No regression on test0, test1
- Code quality maintained

**Why this order?**
- Test2 is trivial (~30-45 min implementation)
- Variables are fundamental for ALL other features
- Quick win builds momentum
- Sets foundation for arithmetic/control flow

---

### **M3: Arithmetic and Comparison Operators**
**Goal:** Basic integer/float arithmetic and comparisons  
**Test Target:** test4, test5, test6  
**Estimated Cycles:** 4-5

**Deliverables:**
- Binary arithmetic operators: +, -, *, /, //, %
- Comparison operators: <, >, <=, >=, ==, !=
- Type coercion (int + float → float)
- Expression evaluation (visitArith_expr, visitTerm, etc.)
- Operator precedence handling

**Acceptance Criteria:**
- test4 passes (case-sensitive variables)
- test5 passes (basic arithmetic)
- test6 passes (comparison operators)
- Proper operator precedence

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

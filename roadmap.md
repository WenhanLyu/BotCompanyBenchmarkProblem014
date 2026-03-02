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

## Current State (Cycle 2)

- **Completion:** ~0-5%
- **Status:** Initial skeleton only - ANTLR infrastructure set up, but EvalVisitor is empty
- **Repository:** All files untracked, no commits yet
- **Analysis Complete:** Team has evaluated requirements, architecture, and test cases

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

### **M1: Foundation & Basic Infrastructure** (NEXT)
**Goal:** Get initial commits in place and basic print working  
**Test Target:** test0 (comments only), test1 (simple print)  
**Estimated Cycles:** 4-6

**Deliverables:**
- Initial commit with .gitignore
- Basic EvalVisitor structure with Value type system
- Implement visitFile_input, visitStmt, visitSimple_stmts
- Implement visitExpr_stmt for print statements
- Support string literals
- Pass test0 (comments) and test1 (print "Hello, World!")

**Acceptance Criteria:**
- Repository has clean git history
- Code compiles successfully
- test0 and test1 produce correct output when run
- No memory leaks (valgrind clean)

---

### **M2: BigInteger Arithmetic**
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

---

### **M3: Core Language Features**
**Goal:** Variables, control flow, basic operators  
**Test Target:** test2-7  
**Estimated Cycles:** 6-8

**Deliverables:**
- Variable assignment and retrieval
- Scope management (global variables accessible everywhere)
- If/elif/else statements
- While loops with break/continue
- Boolean operators (and, or, not)
- Comparison operators
- Arithmetic operators for int/float
- Type conversions and coercions

**Acceptance Criteria:**
- test2-7 pass
- Proper scope behavior (non-standard Python rules)
- Control flow works correctly (break/continue)

---

### **M4: Functions**
**Goal:** Function definition and calls  
**Test Target:** test8-11  
**Estimated Cycles:** 6-8

**Deliverables:**
- Function definition (def)
- Function calls with positional arguments
- Return statements
- Local vs global scope
- Recursion support

**Acceptance Criteria:**
- test8-11 pass
- Functions can call themselves recursively
- Scope isolation works correctly

---

### **M5: Advanced Function Features**
**Goal:** Default arguments, keyword arguments  
**Test Target:** test12-13  
**Estimated Cycles:** 4-6

**Deliverables:**
- Default parameter values
- Keyword arguments
- Mixed positional/keyword argument calls
- Proper argument matching

**Acceptance Criteria:**
- test12-13 pass
- Complex argument matching works correctly

---

### **M6: F-Strings and Advanced Features**
**Goal:** Formatted string literals  
**Test Target:** test14-15  
**Estimated Cycles:** 4-6

**Deliverables:**
- F-string parsing and evaluation
- Expression evaluation inside {}
- Proper formatting (float precision, etc.)
- Nested f-string support
- Multiple assignment
- Tuples

**Acceptance Criteria:**
- test14-15 pass
- F-strings handle all expression types
- Float formatting correct (6 decimal places)

---

### **M7: OJ Submission Preparation**
**Goal:** Full test coverage and submission readiness  
**Test Target:** All 36 local tests  
**Estimated Cycles:** 4-6

**Deliverables:**
- All local tests pass (basic + bigint)
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

### Cycle 1-2: Analysis Phase
- ✅ Comprehensive analysis completed by team (Elena, Marcus, Sophia)
- ✅ BigInteger identified as critical early priority (30% of score)
- ✅ Architecture decisions made (std::variant for Value, exception-based control flow)
- ✅ Test case analysis reveals incremental path from simple to complex
- ⚠️ No expected outputs provided - must use Python 3 as reference

**Key Insight:** Project is essentially at 0% - complete implementation needed. Incremental, test-driven approach is essential.

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

**Active:** None yet  
**Next:** M1 - Foundation & Basic Infrastructure

**Action:** Athena to define M1 milestone for Ares's team when ready.

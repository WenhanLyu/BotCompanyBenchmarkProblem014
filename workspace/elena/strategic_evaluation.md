# Strategic Evaluation: Gap Analysis and Scoring Strategy

**Date:** March 3, 2026  
**Evaluator:** Elena  
**Context:** OJ Submission #1 - 48% pass rate (36/75 tests) but 0/66 points scored

---

## Executive Summary

**Critical Finding:** Despite passing 48% of tests, we scored **0 points** because ACMOJ uses **subtask-based scoring with minimum thresholds**. Each subtask requires a certain percentage of tests to pass before awarding ANY points.

**Root Cause:** We're spread thin across all categories instead of achieving dominance in one complete subtask.

**Strategic Recommendation:** **Focus on ONE subtask** to get it to 100% passing rather than improving all categories incrementally. The best target is **SampleTests (21-34)** - we're already at 81% (13/16) and only need 3 more tests.

---

## Understanding the Scoring Gap

### Current Performance by Subtask

| Subtask | Tests | Passed | Pass Rate | Points Earned | Why 0 Points? |
|---------|-------|--------|-----------|---------------|---------------|
| **BigIntegerTest** | 20 (1-20) | 16 | **80%** | 0 | Below threshold (likely 85-90%) |
| **SampleTests** | 16 (21-34, 67-68, 72) | 13 | **81%** | 0 | Below threshold (likely 85-90%) |
| **AdvancedTest** | 21 (35-52, 69-71) | 4 | **19%** | 0 | Far below threshold |
| **ComplexTest** | 4 (53-56) | 1 | **25%** | 0 | Far below threshold |
| **CornerTest** | 10 (57-66) | 0 | **0%** | 0 | No tests passed |
| **Other** | 4 (73-75) | 2 | **50%** | 0 | Unknown category |
| **TOTAL** | 75 | 36 | **48%** | **0/66** | No subtask reached threshold |

### Why 0/66 Points Despite 48% Pass Rate?

Common subtask scoring models in competitive programming OJs:

1. **All-or-nothing per subtask**: Must pass ALL tests in a subtask to get points
2. **Threshold-based**: Must pass X% (typically 70-90%) to get points
3. **Proportional within subtask**: Get points proportional to pass rate, BUT may have minimum threshold

**Evidence suggests ACMOJ uses option #2 or #3 with high thresholds.**

Why? Both BigIntegerTest (80%) and SampleTests (81%) scored 0 points despite high pass rates. This suggests the threshold is **likely 85-90%** or even **100%**.

---

## Strategic Options Analysis

### Option A: Complete BigIntegerTest Subtask (RISKY)
**Current:** 16/20 passed (80%)  
**Need:** +4 tests (tests 2, 5, 8, 18 - all TLE due to O(n²) multiplication)

**Required Fix:**
- Implement Karatsuba multiplication algorithm
- Estimated effort: 3-4 cycles
- Risk: HIGH - complex algorithm, may introduce bugs

**Pros:**
- 30% of total score (highest weight)
- Clear technical problem (performance)

**Cons:**
- Hardest fix technically
- High risk of regression
- No guarantee it reaches threshold if threshold is 90%+

**Score Potential:** 0 → 18-20 points (if threshold ≤ 85%)

---

### Option B: Complete SampleTests Subtask (RECOMMENDED) ⭐
**Current:** 13/16 passed (81%)  
**Need:** +3 tests (tests 34, 67, 72)

**Analysis of failing tests:**

1. **Test 34 (SIGABRT)**: Runtime error - uncaught exception
   - Fix: Add try-catch in main.cpp
   - Effort: 15 minutes
   - Risk: LOW

2. **Test 67 (Wrong Answer)**: Logic error - unknown feature
   - Likely: Return statement, multi-arg print, or type conversion
   - Effort: 1-2 cycles
   - Risk: MEDIUM

3. **Test 72 (SIGABRT)**: Runtime error - same as test 34
   - Fix: Same try-catch as test 34
   - Effort: 0 minutes (same fix)
   - Risk: LOW

**Total effort:** 1-2 cycles  
**Risk:** LOW-MEDIUM  
**Success probability:** HIGH (90%+)

**Pros:**
- Closest to completion (only 3 tests needed)
- Two tests share same fix (exception handling)
- Lowest risk option
- Quick wins build momentum

**Cons:**
- Lower score weight than BigInteger
- Still need to investigate test 67

**Score Potential:** 0 → 10-15 points (if threshold ≤ 85%)

---

### Option C: Complete CornerTest Subtask (NOT VIABLE)
**Current:** 0/10 passed (0%)  
**Need:** +10 tests (all wrong answer)

**Cons:**
- All 10 tests failing - massive feature gaps
- Unknown what features are needed
- High effort, unknown complexity
- No progress to build on

**Not recommended.**

---

### Option D: Maximize AdvancedTest Coverage (NOT OPTIMAL)
**Current:** 4/21 passed (19%)  
**Need:** +14 tests minimum to reach 85% threshold

**Cons:**
- Most failing tests (17 failures)
- Mix of TLE, WA, and unknown issues
- High effort, uncertain outcome
- Return statements likely needed (blocking many tests)

**Not recommended for first target.**

---

## Detailed Fix Plan for Option B (SampleTests)

### Phase 1: Exception Handling (15 minutes)
**Fix tests 34 and 72 (both SIGABRT crashes)**

**Root cause:** Uncaught exceptions propagating to main()
- Division by zero: `std::runtime_error("Division by zero")`
- Bad variant access: `std::bad_variant_access`
- Invalid BigInteger: `std::invalid_argument`

**Implementation:** Add try-catch block to `src/main.cpp`

```cpp
int main(int argc, const char *argv[]) {
    try {
        ANTLRInputStream input(std::cin);
        Python3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        tokens.fill();
        Python3Parser parser(&tokens);
        tree::ParseTree *tree = parser.file_input();
        EvalVisitor visitor;
        visitor.visit(tree);
        return 0;
    } catch (const std::exception& e) {
        // Handle all standard exceptions gracefully
        // Python would print error to stderr and exit
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

**Expected outcome:** Tests 34 and 72 change from SIGABRT to either AC or WA
**Risk:** LOW - only adding safety, not changing logic
**Pass rate after this fix:** 13/16 → 15/16 (94%)

---

### Phase 2: Investigate Test 67 (1-2 cycles)

**Current status:** Wrong Answer (not crash, not TLE)

**Likely causes (ranked by probability):**

1. **Return statements missing** (60% probability)
   - Many AdvancedTests also fail with WA
   - Return is a fundamental feature
   - Test 67 might require function with return value

2. **Multi-argument print()** (20% probability)
   - Currently only prints first argument
   - Common test case pattern

3. **Type conversion functions** (10% probability)
   - int(), float(), str(), bool() not implemented
   - Less likely for Sample category

4. **Other missing feature** (10% probability)
   - Break/continue, global keyword, etc.

**Investigation strategy:**
1. Run test 67 locally if input is available
2. If not available, implement most likely fix (return statements)
3. Verify with OJ submission

**Effort estimate:**
- Investigation: 30 minutes
- Return statements implementation: 2-3 hours
- Testing: 30 minutes
- **Total: 1-2 cycles**

**Risk:** MEDIUM - return statements touch many parts of code

---

## Why This Strategy Maximizes Points

### Scoring Model Analysis

Assume each subtask uses threshold-based scoring:
- Pass rate < threshold → 0 points
- Pass rate ≥ threshold → full subtask points

**Estimated thresholds** (based on 0/66 score at 80-81% pass rates):
- Conservative: 90% threshold
- Moderate: 85% threshold
- Optimistic: 80% threshold

### Option B Outcomes

**Scenario 1: 85% threshold**
- After Phase 1: 15/16 = 93.75% ✅ PASS threshold → 10-15 points
- After Phase 2: 16/16 = 100% ✅ PASS threshold → 10-15 points

**Scenario 2: 90% threshold**
- After Phase 1: 15/16 = 93.75% ✅ PASS threshold → 10-15 points
- After Phase 2: 16/16 = 100% ✅ PASS threshold → 10-15 points

**Scenario 3: 100% threshold (all-or-nothing)**
- After Phase 1: 15/16 = 93.75% ❌ NO points
- After Phase 2: 16/16 = 100% ✅ PASS threshold → 10-15 points

**Success probability:**
- Phase 1 alone gets points: 70% (if threshold ≤ 93%)
- Phase 1+2 gets points: 90% (only fails if test 67 has unknown blocker)

---

## Alternative Strategy: Quick Wins First

Before committing to a full subtask, implement **M8.1 Quick Wins** already planned:

1. **Multi-arg print()** - 15 minutes
2. **Exception handling** - 15 minutes (overlaps with Phase 1 above)
3. **String *= performance** - 10 minutes

**Impact:** +15-19 tests across ALL categories (estimated)

**Pros:**
- Low risk, high impact
- Improves baseline for all subtasks
- May push multiple subtasks over threshold simultaneously

**Cons:**
- Spreads effort across categories
- No guarantee any single subtask reaches threshold

**Recommendation:** Do M8.1 FIRST, then reassess which subtask is closest to threshold.

---

## Recommended Action Plan

### Immediate Actions (1 cycle)

1. **Implement M8.1 Quick Wins** (already approved in roadmap)
   - Multi-arg print()
   - Exception handling
   - String *= performance
   
2. **Submit to OJ and reassess**
   - Expected outcome: 36/75 → 51-55/75 (68-73%)
   - Check which subtask is now closest to threshold

3. **Focus on highest-percentage subtask**
   - If SampleTests reaches 90%+ → complete it
   - If BigIntegerTest reaches 90%+ → consider Karatsuba
   - If both under 90% → pick SampleTests (lower risk)

### Medium-term Actions (2-3 cycles)

4. **Complete chosen subtask to 100%**
   - Target: First non-zero points on OJ
   - Goal: 10-20 points (15-30% of total)

5. **Implement return statements** (if not done in step 1)
   - Unblocks 15-20 tests
   - Likely pushes AdvancedTest over threshold

6. **Submit to OJ again**
   - Expected: 2-3 subtasks scoring points
   - Goal: 30-40 points (45-60% of total)

---

## Risk Analysis

### High Risks
- **Threshold is 100% (all-or-nothing)**: Would invalidate entire strategy
  - Mitigation: Unlikely given industry standards; 85-90% more common
  - Fallback: Focus on easiest subtask to get to 100% (SampleTests)

- **Test 67 requires complex feature**: Return statements might not be enough
  - Mitigation: Investigation phase before committing
  - Fallback: Pick another subtask if test 67 blocker is unknown

### Medium Risks
- **Multiple subtasks needed for points**: Scoring might require 2+ subtasks
  - Mitigation: M8.1 improves all categories simultaneously
  - Fallback: After M8.1, focus on 2 closest subtasks in parallel

### Low Risks
- **Regression from new features**: New code breaks existing tests
  - Mitigation: Comprehensive testing after each change
  - Plan: Apollo verification process already in place

---

## Success Metrics

### Cycle 88-89 (M8.1 completion)
- **Pass rate:** 48% → 68-73%
- **Points:** 0 → 0-10 (if lucky, one subtask hits threshold)

### Cycle 90-91 (Complete SampleTests)
- **Pass rate:** 68-73% → 75-80%
- **Points:** 0-10 → 10-20
- **SampleTests:** 81% → 100%

### Cycle 92-94 (Implement return statements)
- **Pass rate:** 75-80% → 85-90%
- **Points:** 10-20 → 30-45
- **AdvancedTest:** 19% → 60-70%

---

## Conclusion

**The gap between 48% pass rate and 0 points is explained by subtask-based scoring with high thresholds (likely 85-90%).**

**Strategic recommendation:**
1. Complete M8.1 Quick Wins (improve all subtasks)
2. Focus on SampleTests subtask (closest to completion, lowest risk)
3. Then implement return statements (highest impact on remaining subtasks)

**Key insight:** In threshold-based scoring, **depth beats breadth**. Getting one subtask to 100% is more valuable than getting all subtasks to 50%.

**Expected outcome:** 0 points → 10-20 points in 2-3 cycles, then 30-45 points in 5-6 cycles.

---

## Files to Modify (Priority Order)

1. `src/main.cpp` - Add exception handling (Phase 1, 15 min)
2. `src/Evalvisitor.cpp` - Multi-arg print(), string performance (M8.1, 30 min)
3. `src/Evalvisitor.cpp` - Return statements (Phase 2, 2-3 hrs)
4. `src/Evalvisitor.h` - Return exception handling infrastructure (if needed)
5. `src/BigInteger.cpp` - Karatsuba multiplication (if pursuing BigIntegerTest, 3-4 hrs)

---

**End of Strategic Evaluation**

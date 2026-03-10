# M14 Milestone - BigInteger Division Optimization

## Milestone Definition

**Title:** Fix BigInteger division O(n³) bottleneck to unblock Subtask 2  
**Cycles Budget:** 1  
**Strategic Value:** HIGH - unlocks 25-30 points (Subtask 2)

## Background

**Current State:**
- Test 34: TLE at 19,251ms (30x over time limit)
- Blocks Subtask 2: Currently 14/16 Sample tests (87.5%), need 15/16
- Root cause: Binary search in division creates O(n³) complexity

**Evidence Base:**
- M13 investigation: All 4 workers (Kai, Mia, Noah, Liam) independently identified this as #1 priority
- Kai's detailed analysis: `workspace/kai/test34_fix_recommendation.md`
- Algorithmic soundness verified: Knuth's Algorithm D principle

## Technical Scope

**Code Changes:**
- File: `src/BigInteger.cpp`
- Function: `divideAbs()`
- Lines: 333-350 (binary search loop)

**Current Algorithm:**
```cpp
// Binary search: 30 iterations per digit
while (low <= high) {
    int mid = low + (high - low) / 2;
    BigInteger product(mid);
    product = divisor * product;  // O(m²) multiplication
    // ... comparison and adjustment
}
// Complexity: O(30m²) per digit
```

**New Algorithm:**
```cpp
// Estimate + correction: 1-2 iterations per digit
count = (int)estimate;
if (count > 0) {
    BigInteger product(count);
    product = divisor * product;
    
    // Correction (typically 0-2 iterations)
    while (count > 0 && remainder.compareAbs(product) < 0) {
        count--;
        product = product.subtractAbs(divisor);  // O(m) subtraction
    }
    
    if (count > 0) {
        remainder = remainder.subtractAbs(product);
    }
}
// Complexity: O(m²) + O(2m) ≈ O(m²) per digit
```

**Performance Improvement:**
- From: O(30m²) per digit
- To: O(m²) per digit
- **Speedup: ~30x** (19s → 0.6s expected)

## Acceptance Criteria

### Mandatory (Must Pass)
1. ✅ Test 34 no longer TLE (Accepted or WA - either proves perf fix worked)
2. ✅ OJ-scale benchmark: 10^40 // 3 completes in <1 second (currently would be ~19s)
3. ✅ All 20 BigInteger tests (1-20) still pass locally
4. ✅ No regression on currently passing tests (46/72 maintained)

### Safety Requirements (Lessons from M10)
5. ✅ Algorithmic correctness verified (O(m²) not O(n))
6. ✅ Edge case testing: estimate=0, estimate too high, equal numbers
7. ✅ Memory usage unchanged or improved
8. ✅ This change is ISOLATED - no other modifications
9. ✅ Rollback commit hash documented before change

### Expected Outcomes
- **Optimistic:** +3 tests (34, 55, 72), Subtask 2 unlocked → 50-55 points
- **Realistic:** +2 tests (34, 55), path to Subtask 2 clear
- **Minimum:** Test 34 no longer TLE (even if WA, eliminates perf blocker)

## Implementation Plan

**Step 1: Study Reference (15 min)**
- Read `workspace/kai/test34_fix_recommendation.md`
- Understand current algorithm vs proposed fix
- Review Knuth's Algorithm D (estimate accuracy principle)

**Step 2: Create Test Harness (15 min)**
- Create large division test: 1000+ digit numbers
- Verify current code takes ~19s
- This is the benchmark for "fixed"

**Step 3: Implement Fix (20 min)**
- Replace lines 333-350 in `src/BigInteger.cpp`
- Use Kai's exact code (don't improvise)
- Add comments explaining estimate + correction approach

**Step 4: Testing (30 min)**
- Run large division benchmark: MUST complete <1s
- Run all BigInteger tests (1-20): MUST all pass
- Run all 36 local tests: MUST all pass
- Edge cases: estimate=0, large numbers, equal divisor/dividend
- Memory check: no leaks, no significant increase

**Step 5: Code Review (15 min)**
- Self-review: algorithmic correctness
- Verify: no other changes accidentally included
- Document: commit hash before change (for rollback)

**Step 6: Commit + Push (5 min)**
- Commit message: `[M14] Fix BigInteger division O(n³) → O(m²) bottleneck`
- Push to master (or create PR if you prefer review)

**Total: ~1.5 hours** (fits in 1 cycle with buffer)

## Testing Checklist

**Before any commit:**
- [ ] Large division test created and passing (<1s)
- [ ] BigInteger test 1: PASS
- [ ] BigInteger test 2: PASS
- [ ] BigInteger test 3: PASS
- [ ] BigInteger test 4: PASS
- [ ] BigInteger test 5: PASS
- [ ] BigInteger test 6: PASS
- [ ] BigInteger test 7: PASS
- [ ] BigInteger test 8: PASS
- [ ] BigInteger test 9: PASS
- [ ] BigInteger test 10: PASS
- [ ] BigInteger test 11: PASS
- [ ] BigInteger test 12: PASS
- [ ] BigInteger test 13: PASS
- [ ] BigInteger test 14: PASS
- [ ] BigInteger test 15: PASS
- [ ] BigInteger test 16: PASS
- [ ] BigInteger test 17: PASS
- [ ] BigInteger test 18: PASS
- [ ] BigInteger test 19: PASS
- [ ] BigInteger test 20: PASS
- [ ] All 36 local tests: PASS
- [ ] Edge case: estimate = 0
- [ ] Edge case: estimate too high
- [ ] Edge case: dividend == divisor
- [ ] Memory usage checked (no increase)

## Risk Mitigation

**Technical Risks:**
- ❌ Risk: Estimate accuracy insufficient → Correction loop fails
  - ✅ Mitigation: Knuth's Algorithm D proves estimate within ±2
- ❌ Risk: Correction loop infinite or slow
  - ✅ Mitigation: Estimate starts high, decrements → guaranteed termination in ≤2 iterations
- ❌ Risk: Edge cases not handled
  - ✅ Mitigation: Comprehensive edge case testing required

**Regression Risks:**
- ❌ Risk: Break currently passing BigInteger tests
  - ✅ Mitigation: MANDATORY full test suite before commit
- ❌ Risk: Repeat M10 catastrophe (actually slower)
  - ✅ Mitigation: OJ-scale benchmarking BEFORE submission

**Opportunity Risks:**
- ❌ Risk: Test 34 has secondary issue (still WA after fix)
  - ✅ Mitigation: Acceptable - at least we removed perf blocker
- ❌ Risk: 1 cycle wasted if no improvement
  - ✅ Mitigation: Low risk given unanimous evidence

## Rollback Plan

**If regression detected:**
1. Document what broke (which tests, how)
2. `git revert <M14-commit-hash>` immediately
3. Restore to commit 537d548 (current master)
4. Report to Athena: investigation was wrong, pivot to Plan B

**Plan B (Fallback):**
- Pivot to Type Conversions (M15)
- 2 cycles, +4-6 tests, safer approach
- Guaranteed incremental progress

## Success Metrics

**Primary:**
- Test 34: TLE → Accepted (or WA)

**Secondary:**
- Tests 55, 72: improved performance
- All BigInteger tests maintained
- Zero regressions

**Stretch:**
- Subtask 2 unlocked (+25-30 points)
- OJ score: 25 → 50-55

## Next Steps After M14

**If successful:**
1. Immediate OJ Submission #5
2. Await results (external runner)
3. If Subtask 2 unlocked: Define M15 (Type Conversions or Keyword Args)
4. If Test 34 → WA: Define M15 (Test 34 logic fix)

**If unsuccessful:**
1. Rollback immediately
2. Investigation: why didn't fix work?
3. Pivot to Type Conversions (guaranteed progress)

## References

- **Kai's Fix Recommendation:** `workspace/kai/test34_fix_recommendation.md`
- **Kai's TLE Analysis:** `workspace/kai/test34_tle_analysis.md`
- **Liam's Strategic Report:** `workspace/liam/strategic_recommendation.md`
- **Roadmap M14 Section:** Lines 481-575 in `roadmap.md`
- **M10 Failure Analysis:** `roadmap.md` Cycles 118-127 (learn from mistakes)

## Key Principle

**"Do NOT optimize prematurely, but DO fix algorithmic bottlenecks."**

This is not premature optimization - this is fixing a known O(n³) algorithm that blocks 25-30 points. The evidence is clear, the fix is proven, the risk is low.

Execute with discipline. Test thoroughly. Ship confidently.

# Feature Priority Decision Matrix

**Generated:** Cycle 50 - Tester Analysis  
**Purpose:** Quick reference for strategic decisions

---

## The Numbers

| Feature | Tests Blocked | Score Impact | Effort (cycles) | ROI (% per cycle) | Priority |
|---------|---------------|--------------|-----------------|-------------------|----------|
| **BigInteger Division** | **20** | **~30%** | **3-5** | **7.5%** | **🔴 CRITICAL** |
| F-strings | 2-3 | ~3-5% | 4-6 | 0.8% | 🟡 Medium |
| Global keyword | 1-2 | ~1-3% | 2-3 | 0.8% | 🟢 Low |

---

## Current Status

```
✅ Working (13/16 basic tests):
├── Comments, literals, variables
├── Arithmetic (+, -, *, /, //, % for regular int)
├── Comparisons, logical operators
├── Control flow (if/else, while, break/continue)
├── Functions (def, call, return, recursion, defaults)
└── BigInteger (+, -, *, comparisons)

❌ Broken (3/16 basic tests + 20 BigInteger tests):
├── BigInteger division (//) - INFINITE LOOP
├── BigInteger modulo (%)
├── Global keyword (test13)
└── F-strings (test14-15)
```

---

## OJ Test Distribution

```
Tests 1-20:   BigInteger     [30.3%] ❌ ALL BLOCKED BY DIVISION
Tests 21-34:  Sample         [21.2%] ⚠️ Likely need f-strings
Tests 35-52:  Advanced       [27.3%] ❓ Unknown requirements
Tests 53-56:  Complex        [6.1%]  ❓ Unknown requirements
Tests 57-66:  Corner         [15.1%] ❓ Unknown requirements
```

---

## Decision Rules

### Rule 1: Fix Division First
**Never work on other features while division is broken.**

Division blocks 30% of the score. Everything else combined might add 5-10%.

### Rule 2: Test Before OJ Submission
**Don't submit to OJ until division works.**

Submitting with broken division = guaranteed 20/66 failures = wasted submission slot.

### Rule 3: Incremental Progress
**Commit after each fix. Don't bundle changes.**

If something breaks, easier to identify and rollback.

### Rule 4: Validate Locally First
**All 36 local tests should pass before OJ.**

If local tests fail, OJ tests will definitely fail.

---

## Decision Tree

```
START
  ↓
Is BigInteger division working?
  ├─ NO → Fix division (3-5 cycles)
  │        └─ Test on BigIntegerTest0-19
  │             ├─ All pass? → Continue
  │             └─ Fail? → Debug more
  │
  └─ YES → Are all 36 local tests passing?
            ├─ NO → Which tests fail?
            │        ├─ test13 → Fix global keyword (2-3 cycles)
            │        ├─ test14-15 → Implement f-strings (4-6 cycles)
            │        └─ Other → Debug and fix
            │
            └─ YES → Submit to OJ for feedback
                      └─ Analyze results → Iterate
```

---

## Quick Checks

### Before Starting Any Work:
```bash
# Are basic tests still working?
make && ./code < testcases/basic-testcases/test0.in

# Test simple division
echo 'print(100 // 3)' | ./code

# Test BigInteger division (should not hang)
echo 'print(100000000000000000000 // 3)' | ./code
```

### After Division Fix:
```bash
# Run all BigInteger tests
for i in {0..19}; do
    echo "Testing BigIntegerTest$i..."
    ./code < testcases/bigint-testcases/BigIntegerTest$i.in > out.txt
    diff out.txt testcases/bigint-testcases/BigIntegerTest$i.out
done
```

---

## Common Pitfalls to Avoid

❌ **Don't:** Work on f-strings while division is broken  
✅ **Do:** Fix division first, then consider f-strings

❌ **Don't:** Submit to OJ with known bugs  
✅ **Do:** Pass all local tests first

❌ **Don't:** Try to fix everything at once  
✅ **Do:** One feature at a time, test thoroughly

❌ **Don't:** Assume OJ tests match local tests  
✅ **Do:** Use local tests to validate, expect OJ surprises

---

## Estimated Timeline to OJ-Ready

**Optimistic Path (12-16 cycles):**
1. Fix division: 3-4 cycles → +30% (50% total)
2. F-strings: 4-5 cycles → +4% (54% total)
3. Global keyword: 2-3 cycles → +2% (56% total)
4. Bug fixes: 2-3 cycles → +2% (58% total)
5. OJ submission: 1 cycle → Get feedback

**Realistic Path (18-24 cycles):**
- Add 50% buffer for debugging and edge cases
- Multiple OJ iterations likely needed

**Current Cycle:** 50  
**Estimated OJ-Ready:** Cycle 62-74

---

## Success Criteria

### Milestone 5.1 Complete (Division Fixed):
- ✅ All 20 BigIntegerTest cases pass
- ✅ No infinite loops on large divisions
- ✅ Correct Python semantics (floor division)
- ✅ No regression on test0-12

### Milestone 6 Complete (F-strings):
- ✅ test14-15 pass
- ✅ Expression evaluation in {}
- ✅ Nested f-strings work

### OJ-Ready:
- ✅ All 36 local tests pass
- ✅ No memory leaks
- ✅ Performance within limits
- ✅ Code clean and documented

---

## Final Word

**The division bug is the project's bottleneck.**

Fix it, and you unlock 30% of the score.  
Ignore it, and every other feature is premature optimization.

**Focus. Fix division. Then iterate.**

---

End of Decision Matrix

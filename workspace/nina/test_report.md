# Test Report - Cycle 13
**Date:** 2026-03-02  
**Branch:** leo/variable-storage-assignment  
**Tester:** Nina  

## Summary
- **Build Status:** ✅ PASS (clean compilation with only deprecation warnings)
- **M1 Tests:** ✅ PASS (test0, test1)
- **M2 Tests:** ✅ PASS (test2, test3) - **AHEAD OF SCHEDULE**
- **Additional Passing:** test4, test8 (partially)

## Test Results

### Passing Tests (6/10)
- ✅ test0: Comments only
- ✅ test1: Print string literal
- ✅ test2: Print int, bool, None literals
- ✅ test3: Variable assignment and retrieval
- ✅ test4: Case-sensitive variables
- ✅ test8: If statement (with comparison, but most logic commented out)

### Failing Tests (4/10)
- ❌ test5: Arithmetic operations (1+1 returns 1 instead of 2)
- ❌ test6: Comparison operators (requires arithmetic)
- ❌ test7: String operations
- ❌ test9: While loops

## Detailed Findings

### M2 Status: COMPLETE ✅
The roadmap indicated M2 (Basic Types and Variables) was the next milestone, targeting test2-3. **Both tests are now passing!**

This means:
- ✅ Value type includes: int, bool, None, string
- ✅ Variable storage working (std::map)
- ✅ Variable assignment working
- ✅ Variable lookup working
- ✅ Type-aware printing working

**Bonus:** test4 (case-sensitive variables) also passes, which wasn't explicitly in M2 scope.

### Compilation Status
```
Warnings: 6 deprecation warnings about -Ofast flag (cosmetic only)
Errors: 0
Executable: code (370KB)
```

The `-Ofast` deprecation warnings could be addressed by updating CMakeLists.txt to use `-O3 -ffast-math` instead.

### Next Milestone: M3 (Arithmetic)
Test5 reveals that arithmetic operations are not yet implemented:
- `1 + 1` currently evaluates to `1` (just returns first operand)
- This is expected - M3 scope is to implement arithmetic operators

### Memory Leaks
Not tested (valgrind not available on macOS). Would recommend testing on Linux CI or installing valgrind alternative.

## Recommendations

1. **Celebrate M2 completion** - Leo has completed M2 ahead of expectations
2. **Move to M3** - Implement arithmetic operators per roadmap
3. **Fix deprecation warnings** - Update CMakeLists.txt (low priority)
4. **Consider test8** - It passes but only because comparison logic is commented out. Real test8 will need M3+M4 features.

## Git Status
```
Branch: leo/variable-storage-assignment
Status: Up to date with origin
Untracked files: testcases/test.py, workspace/
```

Clean state, ready for next phase.

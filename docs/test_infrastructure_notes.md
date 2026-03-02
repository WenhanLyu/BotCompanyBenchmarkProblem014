# Test Infrastructure Notes

## Overview

This document describes the test infrastructure setup and known limitations.

## Available Test Outputs

### Basic Tests (testcases/basic-testcases/)

| Test | Status | Notes |
|------|--------|-------|
| test0 | ✅ Generated | Comments only, empty output |
| test1 | ✅ Generated | Simple print statement |
| test2 | ✅ Generated | Variables and arithmetic |
| test3 | ✅ Generated | More arithmetic |
| test4 | ✅ Generated | Boolean operations |
| test5 | ✅ Generated | Comparisons |
| test6 | ✅ Generated | If statements |
| test7 | ✅ Generated | While loops |
| test8 | ✅ Generated | Functions |
| test9 | ✅ Generated | Function calls |
| test10 | ✅ Generated | Recursion |
| test11 | ✅ Generated | More recursion |
| test12 | ✅ Generated | Complex function features |
| test13 | ❌ Failed | Uses custom `random()` function |
| test14 | ✅ Generated | F-strings |
| test15 | ✅ Generated | Advanced features |

**Summary**: 15/16 basic tests have expected outputs (93.75%)

### BigInteger Tests (testcases/bigint-testcases/)

| Tests | Status | Notes |
|-------|--------|-------|
| BigIntegerTest0-19 | ❌ All Failed | Python 3.11+ integer conversion limit |

**Summary**: 0/20 BigInteger tests have expected outputs

## Known Issues

### 1. BigInteger Test Outputs

**Problem**: Python 3.11 and later have a security limit on integer string conversion:

```
SyntaxError: Exceeds the limit (4300 digits) for integer string conversion:
value has 4997 digits; use sys.set_int_max_str_digits() to increase the limit
```

**Workarounds**:

A. Use Python 3.10 or earlier (if available):
```bash
python3.10 testcases/bigint-testcases/BigIntegerTest0.in > testcases/bigint-testcases/BigIntegerTest0.out
```

B. Modify Python 3.11+ to increase limit:
```python
import sys
sys.set_int_max_str_digits(10000)
# Then run the test file
```

C. Wait for actual interpreter implementation and use that as reference

D. Request expected outputs from course staff or OJ

**Recommendation**: For now, develop BigInteger functionality and test against the actual interpreter implementation. Once BigInteger is working, we can generate our own expected outputs.

### 2. test13 (Pollard Rho)

**Problem**: Uses `random(n)` function which is not part of standard Python.

```python
c , p = random(n - 1) + 1 , random(n - 1) + 1
```

This appears to be a built-in function in the simplified Python grammar being implemented.

**Workarounds**:

A. Manually create a Python script that implements `random()`:
```python
import random as r
def random(n):
    return r.randint(0, n)
```

B. Wait for actual interpreter implementation

C. Request expected output from course staff

**Recommendation**: Skip test13 for now. It's 1 out of 16 tests (6.25%) and not critical for M1.

## What Works

The following can be reliably tested with Python 3 as reference:
- Comments (test0)
- Print statements (test1)
- Variables and assignment (test2-3)
- Boolean operations (test4)
- Comparisons (test5)
- Control flow: if/elif/else (test6)
- Loops: while (test7)
- Function definition and calls (test8-12)
- F-strings (test14)
- Advanced features (test15)

This covers all major language features except:
- BigInteger arithmetic
- Custom random() function

## Scripts Available

### test_m1.sh
Automated testing for M1 milestone (test0 and test1).

Usage:
```bash
./test_m1.sh
```

Features:
- Builds project with cmake/make
- Runs tests with 5-second timeout
- Compares output with expected
- Optional valgrind memory leak check
- Color-coded pass/fail reporting

### generate_expected_outputs.sh
Batch generation of expected outputs using Python 3.

Usage:
```bash
./generate_expected_outputs.sh
```

Features:
- Generates .out files for all .in files
- Skips files that already have .out
- Reports success/failure for each test
- Safe to run multiple times

## Recommendations for Team

### For M1 (Foundation)
- Use test0 and test1 (both work perfectly)
- Focus on basic print and comment handling
- test_m1.sh is ready to use

### For M2 (BigInteger)
- Develop BigInteger implementation first
- Test manually with simple cases
- Once working, use it to generate expected outputs
- Or use Python 3.10 if available
- Or request outputs from course staff

### For M3+ (Other Features)
- Use test2-12, test14-15 as references (all generated)
- Skip test13 for now (not critical)
- Incremental testing: add one feature, run corresponding test

### General Approach
1. Implement feature
2. Test with simple manual cases first
3. Run against expected outputs (where available)
4. Compare with Python 3 for validation
5. Debug differences

## Future Improvements

Possible enhancements:
- Create test_m2.sh for BigInteger tests
- Create test_all.sh for comprehensive testing
- Add performance timing to track OJ time limits
- Create custom test runner with better reporting
- Add test coverage tracking

## Questions?

If you encounter issues with test infrastructure:
1. Check this document first
2. Try manual testing with Python 3
3. Check build process (cmake/make)
4. Verify input files are correct
5. Raise issue if infrastructure problem found

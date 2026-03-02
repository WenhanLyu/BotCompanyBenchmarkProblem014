# M1 Testing Guide

## Overview

This document describes the M1 acceptance test infrastructure for the Python Interpreter project.

## Test Coverage

M1 tests cover the most basic functionality:
- **test0**: Empty program with comments only (no output)
- **test1**: Simple print statement (`print("Hello, World!")`)

## Running Tests

### Quick Start

From the project root directory:

```bash
./test_m1.sh
```

This will:
1. Build the project using cmake and make
2. Run test0 and test1
3. Compare outputs with expected results
4. Check for memory leaks (if valgrind is installed)
5. Display a summary

### Expected Output

When all tests pass:

```
=========================================
M1 Acceptance Tests
=========================================

[1/4] Building project...
✓ Build successful

[2/4] Running tests...
  Testing test0... PASS
  Testing test1... PASS

[3/4] Checking for memory leaks...
  Running valgrind on test1...
✓ No memory leaks detected

[4/4] Test Summary
=========================================
Tests passed: 2
Tests failed: 0
=========================================

✓ All M1 acceptance tests passed!
```

### Exit Codes

- `0`: All tests passed
- `1`: One or more tests failed or build error

## Manual Testing

If you want to test manually:

### Build the Project

```bash
cmake .
make
```

This creates an executable named `code` in the project root.

### Run Individual Tests

```bash
# Test 0 (comments only - should produce no output)
./code < testcases/basic-testcases/test0.in

# Test 1 (simple print)
./code < testcases/basic-testcases/test1.in
```

### Expected Outputs

**test0.in:**
```python
#WELCOME！
#HAVE A GOOD DAY!
```

**Expected output:** (empty)

---

**test1.in:**
```python
#Hello, World!
print("Hello, World!")
```

**Expected output:**
```
Hello, World!
```

### Compare with Expected

```bash
# Generate actual output
./code < testcases/basic-testcases/test1.in > /tmp/test1_actual.out

# Compare with expected
diff testcases/basic-testcases/test1.out /tmp/test1_actual.out
```

If there's no output from `diff`, the files match.

## Memory Leak Testing

To check for memory leaks with valgrind:

```bash
valgrind --leak-check=full ./code < testcases/basic-testcases/test1.in
```

Look for:
- `All heap blocks were freed -- no leaks are possible` (good)
- `definitely lost: 0 bytes` (good)

## Generating Expected Outputs

Expected outputs for M1 tests were generated using Python 3:

```bash
cd testcases/basic-testcases
python3 test0.in > test0.out
python3 test1.in > test1.out
```

## Troubleshooting

### Build Fails

1. Check that all ANTLR files are in `generated/`
2. Ensure ANTLR runtime is installed: `sudo apt install ./resources/antlr4-runtime_4.13.1_amd64.deb`
3. Run `cmake .` and `make` manually to see detailed error messages

### Test Fails with "output mismatch"

The script shows the expected vs actual output. Common issues:
- Missing newline at end of output
- Extra spaces or formatting differences
- Incorrect print implementation

### Valgrind Not Found

Install valgrind:
```bash
sudo apt install valgrind
```

Or skip the memory leak check (tests will still run).

### Timeout

If a test times out (default: 5 seconds), there may be:
- Infinite loop in the interpreter
- Deadlock or hang
- Very slow implementation

## Test File Structure

```
testcases/basic-testcases/
├── test0.in          # Input: comments only
├── test0.out         # Expected: empty output
├── test1.in          # Input: print statement
└── test1.out         # Expected: "Hello, World!\n"
```

## Next Steps

After passing M1 tests:
- Move on to M2 (BigInteger arithmetic)
- Run broader test suite with all 36 local tests
- Eventually submit to ACMOJ for full evaluation

## Notes

- M1 tests are minimal and only validate basic infrastructure
- Passing M1 does not guarantee passing all tests
- Each milestone builds on the previous one
- Keep tests passing as you add features (avoid regression)

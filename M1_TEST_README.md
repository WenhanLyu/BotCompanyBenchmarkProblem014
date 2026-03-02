# M1 Acceptance Test - Quick Start

## What is M1?

M1 is the first milestone: **Foundation & Basic Infrastructure**

**Goal**: Get initial commits in place and basic print working

**Test Coverage**:
- test0: Comments only (validates parser handles comments)
- test1: Simple print statement (`print("Hello, World!")`)

## Running M1 Tests

### One Command

```bash
./test_m1.sh
```

That's it! The script will:
1. ✅ Build your code with cmake/make
2. ✅ Run test0 and test1
3. ✅ Compare outputs with expected results
4. ✅ Check for memory leaks (if valgrind installed)
5. ✅ Show you pass/fail results

### Expected Success Output

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
✓ No memory leaks detected

[4/4] Test Summary
=========================================
Tests passed: 2
Tests failed: 0
=========================================

✓ All M1 acceptance tests passed!
```

## What You Need to Implement for M1

To pass M1 tests, your interpreter must:

1. **Handle comments** (lines starting with #)
2. **Support string literals** ("Hello, World!")
3. **Implement print() function** with proper output

That's the minimum! M1 is about getting the foundation working.

## Test Files

**Input files**:
- `testcases/basic-testcases/test0.in` - Comments only
- `testcases/basic-testcases/test1.in` - Print statement

**Expected output files**:
- `testcases/basic-testcases/test0.out` - Empty (no output)
- `testcases/basic-testcases/test1.out` - `Hello, World!\n`

## Troubleshooting

### "cmake failed" or "make failed"
- Make sure your code compiles
- Run `cmake .` and `make` manually to see errors
- Check that ANTLR libraries are installed

### "output mismatch"
The script shows expected vs actual output. Common issues:
- Missing newline after print
- Wrong string literal handling
- Parser not skipping comments

### "executable 'code' not found"
- Check your CMakeLists.txt
- Executable must be named `code` in project root

## Next Steps

After passing M1:
1. ✅ Commit your working code
2. ✅ Move to M2 (BigInteger arithmetic)
3. ✅ Keep running test_m1.sh to avoid regression

## More Information

- **Full testing guide**: [docs/testing_m1.md](docs/testing_m1.md)
- **Test infrastructure notes**: [docs/test_infrastructure_notes.md](docs/test_infrastructure_notes.md)
- **Quick reference**: [TESTING.md](TESTING.md)
- **Project roadmap**: [roadmap.md](roadmap.md)

---

**Ready to start? Just run:**
```bash
./test_m1.sh
```

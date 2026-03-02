# Testing Quick Reference

## M1 Tests (Foundation)

Run M1 acceptance tests:
```bash
./test_m1.sh
```

This tests:
- test0: Comments only (no output)
- test1: Simple print statement

See [docs/testing_m1.md](docs/testing_m1.md) for detailed documentation.

## Manual Testing

### Build
```bash
cmake .
make
```

### Run a single test
```bash
./code < testcases/basic-testcases/test1.in
```

### Check for memory leaks
```bash
valgrind --leak-check=full ./code < testcases/basic-testcases/test1.in
```

## Test Structure

All test cases are in `testcases/`:
- `basic-testcases/` - 16 basic tests (test0-test15)
- `bigint-testcases/` - 20 big integer tests (BigIntegerTest0-BigIntegerTest19)

Each test has:
- `.in` file - Input Python code
- `.out` file - Expected output (generated with Python 3)

## Adding New Expected Outputs

To generate expected output for any test:
```bash
python3 testcases/basic-testcases/testX.in > testcases/basic-testcases/testX.out
```

## Future Milestones

As we progress through milestones, we'll add:
- `test_m2.sh` - BigInteger tests
- `test_m3.sh` - Control flow tests
- `test_all.sh` - Run all local tests

## Resources

- Full testing guide: [docs/testing_m1.md](docs/testing_m1.md)
- Project roadmap: [roadmap.md](roadmap.md)
- Assignment README: [README.md](README.md)

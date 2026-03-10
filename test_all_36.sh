#!/bin/bash
# Test all 36 local tests
# Returns: 0 if all tests pass, 1 if any test fails

set +e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

TESTS_PASSED=0
TESTS_FAILED=0

echo "Running all 36 local tests..."
echo ""

# Run a test
run_test() {
    local test_name=$1
    local test_dir=${2:-"testcases/basic-testcases"}
    local test_input="$test_dir/${test_name}.in"
    local expected_output="$test_dir/${test_name}.out"
    local actual_output="/tmp/${test_name}_actual.out"
    
    echo -n "  ${test_name}... "
    
    if [ ! -f "$test_input" ] || [ ! -f "$expected_output" ]; then
        echo -e "${RED}SKIP (files not found)${NC}"
        return 0
    fi
    
    if ! ./code < "$test_input" > "$actual_output" 2>&1; then
        echo -e "${RED}FAIL (runtime error)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    if diff -q "$expected_output" "$actual_output" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        rm -f "$actual_output"
        return 0
    else
        echo -e "${RED}FAIL (output mismatch)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$actual_output"
        return 1
    fi
}

# Basic tests (test0-test14) - 15 tests
echo "Basic tests (test0-test14):"
for i in {0..14}; do
    run_test "test$i"
done

# Test15 (f-strings) - 1 test
echo ""
echo "F-string test:"
run_test "test15"

# BigInteger tests - 20 tests
echo ""
echo "BigInteger tests:"
if [ -d "testcases/bigint-testcases" ]; then
    for i in {0..19}; do
        run_test "BigIntegerTest$i" "testcases/bigint-testcases"
    done
else
    echo "  BigInteger test directory not found, skipping"
fi

echo ""
echo "========================================="
echo -e "Tests passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Tests failed: ${RED}${TESTS_FAILED}${NC}"
echo "========================================="

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi

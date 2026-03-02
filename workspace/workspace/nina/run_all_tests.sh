#!/bin/bash
# Nina's comprehensive test runner for all basic tests
# Validates string concatenation and checks for regressions

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASSED=0
FAILED=0
TESTS_DIR="testcases/basic-testcases"
WORKSPACE="workspace/workspace/nina"

echo "========================================="
echo "Nina's Full Test Suite Validation"
echo "Issue #35: String Concatenation Validation"
echo "========================================="
echo ""

# Function to run a test
run_test() {
    local test_num=$1
    local test_name="test${test_num}"
    local input_file="${TESTS_DIR}/${test_name}.in"
    local expected_file="${TESTS_DIR}/${test_name}.out"
    local actual_file="${WORKSPACE}/${test_name}_actual.out"
    
    if [ ! -f "$input_file" ]; then
        echo -e "${YELLOW}[SKIP]${NC} ${test_name} - input file not found"
        return
    fi
    
    if [ ! -f "$expected_file" ]; then
        echo -e "${YELLOW}[SKIP]${NC} ${test_name} - expected output not found"
        return
    fi
    
    # Run the test (remove timeout as it may cause issues)
    ./code < "$input_file" > "$actual_file" 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        # Compare outputs
        if diff -q "$expected_file" "$actual_file" > /dev/null 2>&1; then
            echo -e "${GREEN}[PASS]${NC} ${test_name}"
            ((PASSED++))
        else
            echo -e "${RED}[FAIL]${NC} ${test_name} - output mismatch"
            echo "  Expected:"
            head -3 "$expected_file"
            echo "  Actual:"
            head -3 "$actual_file"
            ((FAILED++))
        fi
    else
        echo -e "${RED}[FAIL]${NC} ${test_name} - execution error (exit code: $exit_code)"
        ((FAILED++))
    fi
}

# Run all tests 0-15
for i in {0..15}; do
    run_test $i
done

echo ""
echo "========================================="
echo "Test Results Summary"
echo "========================================="
echo -e "Passed: ${GREEN}${PASSED}${NC}"
echo -e "Failed: ${RED}${FAILED}${NC}"
echo "Total: $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi

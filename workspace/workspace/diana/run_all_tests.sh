#!/bin/bash
# Comprehensive test runner for all basic and BigInteger tests
# This script runs each test and reports PASS/FAIL/TIMEOUT

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
TIMEOUT_TESTS=0

# Results file
RESULTS_FILE="workspace/workspace/diana/test_results.txt"
> "$RESULTS_FILE"

echo "=========================================" | tee -a "$RESULTS_FILE"
echo "COMPREHENSIVE TEST EXECUTION" | tee -a "$RESULTS_FILE"
echo "=========================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

# Function to run a single test
run_test() {
    local test_name=$1
    local test_dir=$2
    local test_input="${test_dir}/${test_name}.in"
    local expected_output="${test_dir}/${test_name}.out"
    local actual_output="/tmp/${test_name}_actual.out"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Testing ${test_name}... " | tee -a "$RESULTS_FILE"
    
    # Check if input file exists
    if [ ! -f "$test_input" ]; then
        echo -e "${RED}FAIL (input not found)${NC}" | tee -a "$RESULTS_FILE"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    # Check if expected output exists
    if [ ! -f "$expected_output" ]; then
        echo -e "${YELLOW}SKIP (expected output not found)${NC}" | tee -a "$RESULTS_FILE"
        return 2
    fi
    
    # Run the interpreter with timeout
    if timeout 10 ./code < "$test_input" > "$actual_output" 2>&1; then
        # Compare outputs
        if diff -q "$expected_output" "$actual_output" > /dev/null 2>&1; then
            echo -e "${GREEN}PASS${NC}" | tee -a "$RESULTS_FILE"
            PASSED_TESTS=$((PASSED_TESTS + 1))
            rm -f "$actual_output"
            return 0
        else
            echo -e "${RED}FAIL (output mismatch)${NC}" | tee -a "$RESULTS_FILE"
            echo "  Expected:" | tee -a "$RESULTS_FILE"
            head -20 "$expected_output" | sed 's/^/    /' | tee -a "$RESULTS_FILE"
            echo "  Got:" | tee -a "$RESULTS_FILE"
            head -20 "$actual_output" | sed 's/^/    /' | tee -a "$RESULTS_FILE"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            rm -f "$actual_output"
            return 1
        fi
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            echo -e "${YELLOW}TIMEOUT${NC}" | tee -a "$RESULTS_FILE"
            TIMEOUT_TESTS=$((TIMEOUT_TESTS + 1))
        else
            echo -e "${RED}FAIL (runtime error)${NC}" | tee -a "$RESULTS_FILE"
            echo "  Error output:" | tee -a "$RESULTS_FILE"
            head -20 "$actual_output" | sed 's/^/    /' | tee -a "$RESULTS_FILE"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
        rm -f "$actual_output"
        return 1
    fi
}

# Run basic tests (test0-test15)
echo -e "${BLUE}=== BASIC TESTS (test0-test15) ===${NC}" | tee -a "$RESULTS_FILE"
for i in {0..15}; do
    run_test "test${i}" "testcases/basic-testcases"
done
echo "" | tee -a "$RESULTS_FILE"

# Run BigInteger tests
echo -e "${BLUE}=== BIGINTEGER TESTS (BigIntegerTest0-19) ===${NC}" | tee -a "$RESULTS_FILE"
for i in {0..19}; do
    run_test "BigIntegerTest${i}" "testcases/bigint-testcases"
done
echo "" | tee -a "$RESULTS_FILE"

# Summary
echo "=========================================" | tee -a "$RESULTS_FILE"
echo "TEST SUMMARY" | tee -a "$RESULTS_FILE"
echo "=========================================" | tee -a "$RESULTS_FILE"
echo "Total tests:   $TOTAL_TESTS" | tee -a "$RESULTS_FILE"
echo -e "Passed:        ${GREEN}${PASSED_TESTS}${NC} ($(echo "scale=1; $PASSED_TESTS * 100 / $TOTAL_TESTS" | bc)%)" | tee -a "$RESULTS_FILE"
echo -e "Failed:        ${RED}${FAILED_TESTS}${NC} ($(echo "scale=1; $FAILED_TESTS * 100 / $TOTAL_TESTS" | bc)%)" | tee -a "$RESULTS_FILE"
echo -e "Timeout:       ${YELLOW}${TIMEOUT_TESTS}${NC} ($(echo "scale=1; $TIMEOUT_TESTS * 100 / $TOTAL_TESTS" | bc)%)" | tee -a "$RESULTS_FILE"
echo "=========================================" | tee -a "$RESULTS_FILE"

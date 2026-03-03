#!/bin/bash
# Comprehensive return statement testing script
# Author: Liam
# Usage: ./test_return_all.sh

set -e  # Exit on error

WORKSPACE="workspace/workspace/liam"
PASSED=0
FAILED=0

echo "================================"
echo "Return Statement Test Suite"
echo "================================"
echo ""

# Function to run a test
run_test() {
    local test_name=$1
    local test_file=$2
    local expected_file=$3
    
    echo -n "Testing $test_name... "
    
    if [ ! -f "$test_file" ]; then
        echo "SKIP (test file not found)"
        return
    fi
    
    if [ ! -f "$expected_file" ]; then
        echo "SKIP (expected output not found)"
        return
    fi
    
    # Run the test
    if ./code < "$test_file" > "${WORKSPACE}/${test_name}_actual.out" 2>&1; then
        # Compare output
        if diff -q "$expected_file" "${WORKSPACE}/${test_name}_actual.out" > /dev/null 2>&1; then
            echo "PASS ✓"
            PASSED=$((PASSED + 1))
        else
            echo "FAIL ✗ (output mismatch)"
            FAILED=$((FAILED + 1))
            echo "  Expected:"
            head -5 "$expected_file" | sed 's/^/    /'
            echo "  Actual:"
            head -5 "${WORKSPACE}/${test_name}_actual.out" | sed 's/^/    /'
            echo "  Run 'diff $expected_file ${WORKSPACE}/${test_name}_actual.out' for details"
        fi
    else
        echo "FAIL ✗ (execution error)"
        FAILED=$((FAILED + 1))
        echo "  Error output:"
        head -5 "${WORKSPACE}/${test_name}_actual.out" | sed 's/^/    /'
    fi
}

# Check if code binary exists
if [ ! -f "./code" ]; then
    echo "ERROR: ./code binary not found. Please build the project first."
    exit 1
fi

# Test 1: Simple returns
run_test "test_return_simple" \
    "${WORKSPACE}/test_return_simple.py" \
    "${WORKSPACE}/test_return_simple.out"

# Test 2: Early returns
run_test "test_return_early" \
    "${WORKSPACE}/test_return_early.py" \
    "${WORKSPACE}/test_return_early.out"

# Test 3: Recursive returns
run_test "test_return_recursive" \
    "${WORKSPACE}/test_return_recursive.py" \
    "${WORKSPACE}/test_return_recursive.out"

# Test 4: test13 (Pollard Rho - full integration)
run_test "test13" \
    "testcases/basic-testcases/test13.in" \
    "${WORKSPACE}/test13_reference.out"

# Summary
echo ""
echo "================================"
echo "Test Summary"
echo "================================"
echo "PASSED: $PASSED"
echo "FAILED: $FAILED"
echo "TOTAL:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "All tests passed! ✓"
    exit 0
else
    echo "Some tests failed. ✗"
    exit 1
fi

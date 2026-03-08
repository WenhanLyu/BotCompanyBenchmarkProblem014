#!/bin/bash
# Test script for break and continue validation (with stdin redirection)

WORKSPACE="workspace/nina"
PASS=0
FAIL=0

echo "========================================"
echo "Break/Continue Statement Test Suite"
echo "========================================"
echo ""

# Function to run a single test
run_test() {
    local test_name=$1
    local test_file="$WORKSPACE/$test_name.py"
    
    echo "Testing: $test_name"
    
    # Generate expected output with Python
    python3 "$test_file" > "$WORKSPACE/${test_name}_expected.out" 2>&1
    
    # Generate actual output with our interpreter (using stdin redirection)
    ./code < "$test_file" > "$WORKSPACE/${test_name}_actual.out" 2>&1
    
    # Compare outputs
    if diff -q "$WORKSPACE/${test_name}_expected.out" "$WORKSPACE/${test_name}_actual.out" > /dev/null; then
        echo "  ✅ PASS"
        PASS=$((PASS + 1))
    else
        echo "  ❌ FAIL"
        echo "  Expected:"
        cat "$WORKSPACE/${test_name}_expected.out"
        echo "  Actual:"
        cat "$WORKSPACE/${test_name}_actual.out"
        echo "  Diff:"
        diff "$WORKSPACE/${test_name}_expected.out" "$WORKSPACE/${test_name}_actual.out"
        FAIL=$((FAIL + 1))
    fi
    echo ""
}

# Run all tests
run_test "test_break_simple"
run_test "test_continue_simple"
run_test "test_break_nested"
run_test "test_continue_nested"
run_test "test_break_first"
run_test "test_continue_last"
run_test "test_break_continue_mixed"

echo "========================================"
echo "Summary: $PASS passed, $FAIL failed"
echo "========================================"

exit $FAIL

#!/bin/bash
# Comprehensive regression test for basic testcases

PASS=0
FAIL=0

echo "========================================"
echo "Regression Test Suite - Basic Tests"
echo "========================================"
echo ""

run_test() {
    local test_name=$1
    local test_input="testcases/basic-testcases/${test_name}.in"
    local expected_output="testcases/basic-testcases/${test_name}.out"
    local actual_output="workspace/nina/${test_name}_actual.out"
    
    # Skip if input doesn't exist
    if [ ! -f "$test_input" ]; then
        return
    fi
    
    # Skip if expected output doesn't exist (like test13)
    if [ ! -f "$expected_output" ]; then
        echo "  $test_name: SKIPPED (no expected output)"
        return
    fi
    
    echo -n "  Testing $test_name... "
    
    # Run the interpreter
    if ./code < "$test_input" > "$actual_output" 2>&1; then
        # Compare outputs
        if diff -q "$expected_output" "$actual_output" > /dev/null 2>&1; then
            echo "✅ PASS"
            PASS=$((PASS + 1))
        else
            echo "❌ FAIL (output mismatch)"
            FAIL=$((FAIL + 1))
        fi
    else
        echo "❌ FAIL (runtime error)"
        FAIL=$((FAIL + 1))
    fi
}

# Run all basic tests
for i in {0..15}; do
    run_test "test$i"
done

echo ""
echo "========================================"
echo "Summary: $PASS passed, $FAIL failed"
echo "========================================"

exit $FAIL

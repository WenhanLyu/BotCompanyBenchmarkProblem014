#!/bin/bash
# Regression test runner for M4.3

TESTS="0 1 2 3 4 5 6 7 8 9 10 12"
PASSED=0
FAILED=0
FAILED_TESTS=""

for i in $TESTS; do
    TEST_IN="testcases/basic-testcases/test${i}.in"
    TEST_OUT="testcases/basic-testcases/test${i}.out"
    ACTUAL_OUT="workspace/nina/test${i}_actual.out"
    
    if [ ! -f "$TEST_IN" ]; then
        echo "⚠️  test${i}: SKIP (input file not found)"
        continue
    fi
    
    if [ ! -f "$TEST_OUT" ]; then
        echo "⚠️  test${i}: SKIP (expected output not found)"
        continue
    fi
    
    # Run the test
    cat "$TEST_IN" | ./code > "$ACTUAL_OUT" 2>&1
    
    # Compare outputs
    if diff -q "$ACTUAL_OUT" "$TEST_OUT" > /dev/null 2>&1; then
        echo "✅ test${i}: PASS"
        PASSED=$((PASSED + 1))
    else
        echo "❌ test${i}: FAIL"
        FAILED=$((FAILED + 1))
        FAILED_TESTS="$FAILED_TESTS test${i}"
        echo "   Expected:"
        head -3 "$TEST_OUT"
        echo "   Actual:"
        head -3 "$ACTUAL_OUT"
        echo ""
    fi
done

echo ""
echo "====== SUMMARY ======"
echo "Passed: $PASSED"
echo "Failed: $FAILED"
if [ $FAILED -gt 0 ]; then
    echo "Failed tests:$FAILED_TESTS"
fi

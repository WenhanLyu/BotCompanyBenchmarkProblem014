#!/bin/bash
# Comprehensive test script for all 20 BigIntegerTest cases
# Uses stdin method (not file input which is broken)

TEST_DIR="testcases/bigint-testcases"
EXECUTABLE="./code"
TOTAL_TESTS=20
PASS_COUNT=0
FAIL_COUNT=0

# Output files
PASS_LIST="workspace/workspace/nina/passing_tests.txt"
FAIL_LIST="workspace/workspace/nina/failing_tests.txt"
REPORT="workspace/workspace/nina/biginteger_test_report.md"

# Create workspace directory
mkdir -p workspace/workspace/nina

# Clear previous results
> "$PASS_LIST"
> "$FAIL_LIST"
> "$REPORT"

echo "========================================" | tee -a "$REPORT"
echo "BigInteger Comprehensive Test Report" | tee -a "$REPORT"
echo "Testing all 20 BigIntegerTest cases" | tee -a "$REPORT"
echo "Method: stdin (cat input | ./code /dev/stdin)" | tee -a "$REPORT"
echo "========================================" | tee -a "$REPORT"
echo "" | tee -a "$REPORT"

# Test each case
for i in {0..19}; do
    TEST_NAME="BigIntegerTest${i}"
    INPUT_FILE="${TEST_DIR}/${TEST_NAME}.in"
    EXPECTED_FILE="${TEST_DIR}/${TEST_NAME}.out"
    ACTUAL_FILE="workspace/workspace/nina/${TEST_NAME}_actual.out"
    
    echo -n "Testing ${TEST_NAME}... "
    
    # Run test using stdin method (correct method)
    if cat "$INPUT_FILE" | "$EXECUTABLE" /dev/stdin > "$ACTUAL_FILE" 2>&1; then
        # Compare output
        if diff -q "$EXPECTED_FILE" "$ACTUAL_FILE" > /dev/null 2>&1; then
            echo "PASS" | tee -a "$PASS_LIST"
            echo "✅ ${TEST_NAME}: PASS" >> "$REPORT"
            ((PASS_COUNT++))
        else
            echo "FAIL (output mismatch)" | tee -a "$FAIL_LIST"
            echo "❌ ${TEST_NAME}: FAIL (output mismatch)" >> "$REPORT"
            echo "   Diff (first 20 lines):" >> "$REPORT"
            diff "$EXPECTED_FILE" "$ACTUAL_FILE" | head -20 >> "$REPORT"
            echo "" >> "$REPORT"
            ((FAIL_COUNT++))
        fi
    else
        EXIT_CODE=$?
        echo "FAIL (runtime error, exit code $EXIT_CODE)" | tee -a "$FAIL_LIST"
        echo "❌ ${TEST_NAME}: FAIL (runtime error, exit code $EXIT_CODE)" >> "$REPORT"
        echo "   Output:" >> "$REPORT"
        head -20 "$ACTUAL_FILE" >> "$REPORT"
        echo "" >> "$REPORT"
        ((FAIL_COUNT++))
    fi
done

# Summary
echo "" | tee -a "$REPORT"
echo "========================================" | tee -a "$REPORT"
echo "Summary" | tee -a "$REPORT"
echo "========================================" | tee -a "$REPORT"
echo "Total Tests: $TOTAL_TESTS" | tee -a "$REPORT"
echo "Passed: $PASS_COUNT" | tee -a "$REPORT"
echo "Failed: $FAIL_COUNT" | tee -a "$REPORT"
echo "" | tee -a "$REPORT"

if [ $PASS_COUNT -eq $TOTAL_TESTS ]; then
    echo "🎉 ALL TESTS PASSED! Milestone 5 COMPLETE!" | tee -a "$REPORT"
    exit 0
else
    echo "⚠️  Some tests failed. See details above." | tee -a "$REPORT"
    exit 1
fi

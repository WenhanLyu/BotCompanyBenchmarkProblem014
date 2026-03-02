#!/bin/bash
# Comprehensive test script for all 20 BigIntegerTest cases
# Simplified version without timeout (for macOS compatibility)

TEST_DIR="testcases/bigint-testcases"
EXECUTABLE="./code"
TOTAL_TESTS=20
PASS_COUNT=0
FAIL_COUNT=0

# Output files
WORKSPACE="workspace/workspace/nina"
PASS_LIST="$WORKSPACE/passing_tests.txt"
FAIL_LIST="$WORKSPACE/failing_tests.txt"
REPORT="$WORKSPACE/biginteger_test_report.md"

# Create workspace directory
mkdir -p "$WORKSPACE"

# Clear previous results
> "$PASS_LIST"
> "$FAIL_LIST"
> "$REPORT"

echo "========================================" | tee -a "$REPORT"
echo "BigInteger Comprehensive Test Report" | tee -a "$REPORT"
echo "Testing all 20 BigIntegerTest cases" | tee -a "$REPORT"
echo "Method: stdin (cat input | ./code /dev/stdin)" | tee -a "$REPORT"
echo "Date: $(date)" | tee -a "$REPORT"
echo "========================================" | tee -a "$REPORT"
echo "" | tee -a "$REPORT"

# Test each case
for i in {0..19}; do
    TEST_NAME="BigIntegerTest${i}"
    INPUT_FILE="${TEST_DIR}/${TEST_NAME}.in"
    EXPECTED_FILE="${TEST_DIR}/${TEST_NAME}.out"
    ACTUAL_FILE="$WORKSPACE/${TEST_NAME}_actual.out"
    
    printf "Testing ${TEST_NAME}... "
    
    # Run test using stdin method (correct method)
    if cat "$INPUT_FILE" | "$EXECUTABLE" /dev/stdin > "$ACTUAL_FILE" 2>&1; then
        # Compare output
        if diff -q "$EXPECTED_FILE" "$ACTUAL_FILE" > /dev/null 2>&1; then
            echo "PASS"
            echo "$TEST_NAME" >> "$PASS_LIST"
            echo "✅ ${TEST_NAME}: PASS" >> "$REPORT"
            ((PASS_COUNT++))
        else
            echo "FAIL (output mismatch)"
            echo "$TEST_NAME" >> "$FAIL_LIST"
            echo "❌ ${TEST_NAME}: FAIL (output mismatch)" >> "$REPORT"
            echo "" >> "$REPORT"
            echo "Expected output (first 10 lines):" >> "$REPORT"
            head -10 "$EXPECTED_FILE" >> "$REPORT"
            echo "" >> "$REPORT"
            echo "Actual output (first 10 lines):" >> "$REPORT"
            head -10 "$ACTUAL_FILE" >> "$REPORT"
            echo "" >> "$REPORT"
            echo "Diff (first 30 lines):" >> "$REPORT"
            diff "$EXPECTED_FILE" "$ACTUAL_FILE" | head -30 >> "$REPORT"
            echo "" >> "$REPORT"
            ((FAIL_COUNT++))
        fi
    else
        EXIT_CODE=$?
        echo "FAIL (runtime error, exit code $EXIT_CODE)"
        echo "$TEST_NAME" >> "$FAIL_LIST"
        echo "❌ ${TEST_NAME}: FAIL (runtime error, exit code $EXIT_CODE)" >> "$REPORT"
        echo "   Output (first 10 lines):" >> "$REPORT"
        head -10 "$ACTUAL_FILE" 2>/dev/null >> "$REPORT"
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
echo "Pass Rate: $(awk "BEGIN {printf \"%.1f%%\", ($PASS_COUNT/$TOTAL_TESTS)*100}")" | tee -a "$REPORT"
echo "" | tee -a "$REPORT"

if [ $PASS_COUNT -eq $TOTAL_TESTS ]; then
    echo "🎉 ALL TESTS PASSED! Milestone 5 COMPLETE!" | tee -a "$REPORT"
    exit 0
else
    echo "⚠️  $FAIL_COUNT test(s) failed. See report for details." | tee -a "$REPORT"
    echo "" | tee -a "$REPORT"
    echo "Passing tests:" | tee -a "$REPORT"
    cat "$PASS_LIST" 2>/dev/null | tee -a "$REPORT"
    echo "" | tee -a "$REPORT"
    echo "Failing tests:" | tee -a "$REPORT"
    cat "$FAIL_LIST" 2>/dev/null | tee -a "$REPORT"
    exit 1
fi

#!/bin/bash

WORKSPACE_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/willow"
mkdir -p "$WORKSPACE_DIR"

REPORT="$WORKSPACE_DIR/willow_bigint_tests.md"

echo "# BigInteger Test Execution Report" > "$REPORT"
echo "" >> "$REPORT"
echo "**Executor:** Willow" >> "$REPORT"
echo "**Date:** $(date)" >> "$REPORT"
echo "**Repository:** tbc-pdb-014" >> "$REPORT"
echo "" >> "$REPORT"

echo "## Build Status" >> "$REPORT"
echo "" >> "$REPORT"
if [ -x ./code ]; then
    echo "✅ **Build successful** - Interpreter executable found at ./code" >> "$REPORT"
else
    echo "❌ **Build failed** - Interpreter not found" >> "$REPORT"
    exit 1
fi
echo "" >> "$REPORT"

echo "## Test Results" >> "$REPORT"
echo "" >> "$REPORT"

PASS_COUNT=0
FAIL_COUNT=0
CRITICAL_TESTS=(2 5 8 18)

for i in {0..19}; do
    TEST_NAME="BigIntegerTest$i"
    INPUT_FILE="testcases/bigint-testcases/${TEST_NAME}.in"
    EXPECTED_FILE="testcases/bigint-testcases/${TEST_NAME}.out"
    OUTPUT_FILE="$WORKSPACE_DIR/test${i}.out"
    
    echo "Running $TEST_NAME..."
    
    # Measure execution time
    START_TIME=$(date +%s.%N)
    ./code "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
    EXIT_CODE=$?
    END_TIME=$(date +%s.%N)
    
    EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc)
    
    # Check if output matches expected
    if diff -q "$OUTPUT_FILE" "$EXPECTED_FILE" > /dev/null 2>&1; then
        STATUS="✅ PASS"
        ((PASS_COUNT++))
    else
        STATUS="❌ FAIL"
        ((FAIL_COUNT++))
    fi
    
    # Mark critical tests
    IS_CRITICAL=""
    for crit in "${CRITICAL_TESTS[@]}"; do
        if [ "$i" -eq "$crit" ]; then
            IS_CRITICAL=" (Critical - previously had TLE)"
            break
        fi
    done
    
    # Check if time exceeded 1 second
    TIME_STATUS=""
    if (( $(echo "$EXEC_TIME > 1.0" | bc -l) )); then
        TIME_STATUS=" ⚠️ SLOW (>1s)"
    fi
    
    echo "### $TEST_NAME$IS_CRITICAL" >> "$REPORT"
    echo "- **Status:** $STATUS" >> "$REPORT"
    echo "- **Execution Time:** ${EXEC_TIME}s$TIME_STATUS" >> "$REPORT"
    echo "- **Exit Code:** $EXIT_CODE" >> "$REPORT"
    echo "" >> "$REPORT"
done

echo "## Summary" >> "$REPORT"
echo "" >> "$REPORT"
echo "**Total Tests:** 20" >> "$REPORT"
echo "**Passed:** $PASS_COUNT" >> "$REPORT"
echo "**Failed:** $FAIL_COUNT" >> "$REPORT"
echo "" >> "$REPORT"

if [ $PASS_COUNT -eq 20 ]; then
    echo "✅ **ALL TESTS PASSED**" >> "$REPORT"
    echo "" >> "$REPORT"
    echo "All 20 BigInteger tests completed successfully with correct output." >> "$REPORT"
else
    echo "❌ **SOME TESTS FAILED**" >> "$REPORT"
    echo "" >> "$REPORT"
    echo "$FAIL_COUNT test(s) did not produce the expected output." >> "$REPORT"
fi

cat "$REPORT"

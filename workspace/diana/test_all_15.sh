#!/bin/bash
# Test all BigInteger tests 0-15 and create summary

RESULTS_FILE="workspace/diana/test_results_summary.txt"

> "$RESULTS_FILE"

echo "BigInteger Tests 0-15 Summary" | tee -a "$RESULTS_FILE"
echo "=============================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

PASS_COUNT=0
FAIL_COUNT=0
ERROR_COUNT=0

for i in {0..15}; do
    printf "Test %2d: " "$i" | tee -a "$RESULTS_FILE"
    
    INPUT="testcases/bigint-testcases/BigIntegerTest${i}.in"
    EXPECTED="testcases/bigint-testcases/BigIntegerTest${i}.out"
    
    if [ ! -f "$INPUT" ] || [ ! -f "$EXPECTED" ]; then
        echo "SKIP (files missing)" | tee -a "$RESULTS_FILE"
        continue
    fi
    
    ACTUAL=$(./code < "$INPUT" 2>&1)
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "ERROR (exit code $EXIT_CODE)" | tee -a "$RESULTS_FILE"
        ERROR_COUNT=$((ERROR_COUNT + 1))
        continue
    fi
    
    EXPECTED_TEXT=$(cat "$EXPECTED")
    
    if [ "$ACTUAL" = "$EXPECTED_TEXT" ]; then
        echo "PASS" | tee -a "$RESULTS_FILE"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL" | tee -a "$RESULTS_FILE"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
done

echo "" | tee -a "$RESULTS_FILE"
echo "=============================" | tee -a "$RESULTS_FILE"
echo "Results: $PASS_COUNT passed, $FAIL_COUNT failed, $ERROR_COUNT errors" | tee -a "$RESULTS_FILE"

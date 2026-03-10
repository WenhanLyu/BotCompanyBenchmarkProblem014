#!/bin/bash

# Comprehensive Test Verification Script
# Tests all 16 basic tests (test0-15) and all 20 BigInteger tests (BigIntegerTest0-19)
# Compares against M11 completion report claims

RESULTS_FILE="workspace/sage/test_results.txt"
SUMMARY_FILE="workspace/sage/test_summary.md"
RUN_SCRIPT="workspace/sage/run_single_test.sh"

echo "=== COMPREHENSIVE TEST VERIFICATION ===" > "$RESULTS_FILE"
echo "Started: $(date)" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Counters
BASIC_PASS=0
BASIC_FAIL=0
BIGINT_PASS=0
BIGINT_FAIL=0

echo "## BASIC TESTS (test0-15)" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Run basic tests (test0-15)
for i in {0..15}; do
    TEST_NAME="test$i"
    INPUT_FILE="testcases/basic-testcases/${TEST_NAME}.in"
    EXPECTED_FILE="testcases/basic-testcases/${TEST_NAME}.out"
    OUTPUT_FILE="workspace/sage/${TEST_NAME}_output.txt"
    
    if [ ! -f "$INPUT_FILE" ]; then
        echo "[$TEST_NAME] SKIP - input file not found" >> "$RESULTS_FILE"
        continue
    fi
    
    if [ ! -f "$EXPECTED_FILE" ]; then
        echo "[$TEST_NAME] SKIP - expected output not found" >> "$RESULTS_FILE"
        continue
    fi
    
    # Run test
    START_TIME=$(date +%s.%N)
    "$RUN_SCRIPT" "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
    EXIT_CODE=$?
    END_TIME=$(date +%s.%N)
    DURATION=$(echo "$END_TIME - $START_TIME" | bc)
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "[$TEST_NAME] FAIL - Runtime error (exit code $EXIT_CODE) [${DURATION}s]" >> "$RESULTS_FILE"
        BASIC_FAIL=$((BASIC_FAIL + 1))
    else
        # Compare output
        if diff -q "$OUTPUT_FILE" "$EXPECTED_FILE" > /dev/null 2>&1; then
            printf "[$TEST_NAME] PASS [%.3fs]\n" "$DURATION" >> "$RESULTS_FILE"
            BASIC_PASS=$((BASIC_PASS + 1))
        else
            echo "[$TEST_NAME] FAIL - Output mismatch [${DURATION}s]" >> "$RESULTS_FILE"
            BASIC_FAIL=$((BASIC_FAIL + 1))
        fi
    fi
done

echo "" >> "$RESULTS_FILE"
echo "## BIGINTEGER TESTS (BigIntegerTest0-19)" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Run BigInteger tests
for i in {0..19}; do
    TEST_NAME="BigIntegerTest$i"
    INPUT_FILE="testcases/bigint-testcases/${TEST_NAME}.in"
    EXPECTED_FILE="testcases/bigint-testcases/${TEST_NAME}.out"
    OUTPUT_FILE="workspace/sage/${TEST_NAME}_output.txt"
    
    if [ ! -f "$INPUT_FILE" ]; then
        echo "[$TEST_NAME] SKIP - input file not found" >> "$RESULTS_FILE"
        continue
    fi
    
    if [ ! -f "$EXPECTED_FILE" ]; then
        echo "[$TEST_NAME] SKIP - expected output not found" >> "$RESULTS_FILE"
        continue
    fi
    
    # Run test
    START_TIME=$(date +%s.%N)
    "$RUN_SCRIPT" "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
    EXIT_CODE=$?
    END_TIME=$(date +%s.%N)
    DURATION=$(echo "$END_TIME - $START_TIME" | bc)
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "[$TEST_NAME] FAIL - Runtime error (exit code $EXIT_CODE) [${DURATION}s]" >> "$RESULTS_FILE"
        BIGINT_FAIL=$((BIGINT_FAIL + 1))
    else
        # Compare output
        if diff -q "$OUTPUT_FILE" "$EXPECTED_FILE" > /dev/null 2>&1; then
            printf "[$TEST_NAME] PASS [%.3fs]\n" "$DURATION" >> "$RESULTS_FILE"
            BIGINT_PASS=$((BIGINT_PASS + 1))
        else
            echo "[$TEST_NAME] FAIL - Output mismatch [${DURATION}s]" >> "$RESULTS_FILE"
            BIGINT_FAIL=$((BIGINT_FAIL + 1))
        fi
    fi
done

echo "" >> "$RESULTS_FILE"
echo "=== SUMMARY ===" >> "$RESULTS_FILE"
echo "Basic Tests: $BASIC_PASS pass, $BASIC_FAIL fail (Total: $((BASIC_PASS + BASIC_FAIL)))" >> "$RESULTS_FILE"
echo "BigInteger Tests: $BIGINT_PASS pass, $BIGINT_FAIL fail (Total: $((BIGINT_PASS + BIGINT_FAIL)))" >> "$RESULTS_FILE"
echo "Overall: $((BASIC_PASS + BIGINT_PASS)) pass, $((BASIC_FAIL + BIGINT_FAIL)) fail" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"
echo "Completed: $(date)" >> "$RESULTS_FILE"

# Display results
cat "$RESULTS_FILE"

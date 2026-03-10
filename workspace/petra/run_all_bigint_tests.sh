#!/bin/bash

# Script to run all BigInteger tests (0-19) and collect results
# Created by Petra - BigInteger Test Execution Specialist

# Get the absolute path to the project directory
PROJECT_DIR="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014"
TESTDIR="$PROJECT_DIR/testcases/bigint-testcases"
INTERPRETER="$PROJECT_DIR/code"
RESULTS_FILE="$PROJECT_DIR/workspace/petra/test_results.txt"
DETAILED_LOG="$PROJECT_DIR/workspace/petra/detailed_test_log.txt"

# Clear previous results
> "$RESULTS_FILE"
> "$DETAILED_LOG"

echo "================================" | tee -a "$RESULTS_FILE"
echo "BigInteger Test Execution Report" | tee -a "$RESULTS_FILE"
echo "Date: $(date)" | tee -a "$RESULTS_FILE"
echo "Interpreter: $INTERPRETER" | tee -a "$RESULTS_FILE"
echo "================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

passed=0
failed=0
total=20

for i in {0..19}; do
    test_name="BigIntegerTest$i"
    input_file="$TESTDIR/${test_name}.in"
    expected_file="$TESTDIR/${test_name}.out"
    
    echo "Running $test_name..." | tee -a "$DETAILED_LOG"
    
    if [ ! -f "$input_file" ]; then
        echo "  ERROR: Input file $input_file not found" | tee -a "$RESULTS_FILE" "$DETAILED_LOG"
        ((failed++))
        continue
    fi
    
    if [ ! -f "$expected_file" ]; then
        echo "  ERROR: Expected output file $expected_file not found" | tee -a "$RESULTS_FILE" "$DETAILED_LOG"
        ((failed++))
        continue
    fi
    
    # Run the test and capture output
    start_time=$(date +%s)
    actual_output=$("$INTERPRETER" < "$input_file" 2>&1)
    exit_code=$?
    end_time=$(date +%s)
    elapsed=$((end_time - start_time))
    
    # Check for errors
    if [ $exit_code -ne 0 ]; then
        echo "  FAIL: Interpreter returned exit code $exit_code" | tee -a "$RESULTS_FILE" "$DETAILED_LOG"
        echo "  Output: $actual_output" >> "$DETAILED_LOG"
        ((failed++))
        continue
    fi
    
    # Read expected output
    expected_output=$(cat "$expected_file")
    
    # Compare outputs
    if [ "$actual_output" = "$expected_output" ]; then
        echo "  PASS (${elapsed}s)" | tee -a "$RESULTS_FILE" "$DETAILED_LOG"
        ((passed++))
    else
        echo "  FAIL: Output mismatch" | tee -a "$RESULTS_FILE" "$DETAILED_LOG"
        echo "  Expected:" >> "$DETAILED_LOG"
        echo "$expected_output" >> "$DETAILED_LOG"
        echo "  Actual:" >> "$DETAILED_LOG"
        echo "$actual_output" >> "$DETAILED_LOG"
        echo "" >> "$DETAILED_LOG"
        ((failed++))
    fi
done

echo "" | tee -a "$RESULTS_FILE"
echo "================================" | tee -a "$RESULTS_FILE"
echo "Summary:" | tee -a "$RESULTS_FILE"
echo "  Total tests: $total" | tee -a "$RESULTS_FILE"
echo "  Passed: $passed" | tee -a "$RESULTS_FILE"
echo "  Failed: $failed" | tee -a "$RESULTS_FILE"
echo "================================" | tee -a "$RESULTS_FILE"

if [ $failed -eq 0 ]; then
    echo "✓ ALL TESTS PASSED!" | tee -a "$RESULTS_FILE"
    exit 0
else
    echo "✗ SOME TESTS FAILED" | tee -a "$RESULTS_FILE"
    exit 1
fi

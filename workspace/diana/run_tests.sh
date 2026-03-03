#!/bin/bash
# Script to run BigInteger tests 0-15 and capture results

TESTDIR="testcases/bigint-testcases"
BINARY="./code"
RESULTS_FILE="workspace/diana/test_results.txt"

# Clear previous results
> "$RESULTS_FILE"

echo "Running BigInteger Tests 0-15..." | tee -a "$RESULTS_FILE"
echo "================================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

for i in {0..15}; do
    echo "=== Test $i ===" | tee -a "$RESULTS_FILE"
    
    INPUT="$TESTDIR/BigIntegerTest${i}.in"
    EXPECTED="$TESTDIR/BigIntegerTest${i}.out"
    
    if [ ! -f "$INPUT" ]; then
        echo "SKIP: Input file not found" | tee -a "$RESULTS_FILE"
        echo "" | tee -a "$RESULTS_FILE"
        continue
    fi
    
    if [ ! -f "$EXPECTED" ]; then
        echo "SKIP: Expected output file not found" | tee -a "$RESULTS_FILE"
        echo "" | tee -a "$RESULTS_FILE"
        continue
    fi
    
    # Run test with timeout (using gtimeout if available, otherwise plain run)
    if command -v gtimeout &> /dev/null; then
        ACTUAL=$(gtimeout 5 "$BINARY" < "$INPUT" 2>&1)
        EXIT_CODE=$?
        if [ $EXIT_CODE -eq 124 ]; then
            echo "TIMEOUT: Test exceeded 5 seconds" | tee -a "$RESULTS_FILE"
            echo "" | tee -a "$RESULTS_FILE"
            continue
        fi
    else
        # No timeout available, run directly but warn
        ACTUAL=$("$BINARY" < "$INPUT" 2>&1)
        EXIT_CODE=$?
    fi
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "ERROR: Program crashed or returned non-zero exit code ($EXIT_CODE)" | tee -a "$RESULTS_FILE"
        echo "Output:" | tee -a "$RESULTS_FILE"
        echo "$ACTUAL" | tee -a "$RESULTS_FILE"
        echo "" | tee -a "$RESULTS_FILE"
        continue
    fi
    
    # Compare outputs
    EXPECTED_TEXT=$(cat "$EXPECTED")
    
    if [ "$ACTUAL" = "$EXPECTED_TEXT" ]; then
        echo "PASS" | tee -a "$RESULTS_FILE"
    else
        echo "FAIL" | tee -a "$RESULTS_FILE"
        echo "Expected:" | tee -a "$RESULTS_FILE"
        echo "$EXPECTED_TEXT" | head -20 | tee -a "$RESULTS_FILE"
        if [ $(echo "$EXPECTED_TEXT" | wc -l) -gt 20 ]; then
            echo "... (truncated)" | tee -a "$RESULTS_FILE"
        fi
        echo "" | tee -a "$RESULTS_FILE"
        echo "Actual:" | tee -a "$RESULTS_FILE"
        echo "$ACTUAL" | head -20 | tee -a "$RESULTS_FILE"
        if [ $(echo "$ACTUAL" | wc -l) -gt 20 ]; then
            echo "... (truncated)" | tee -a "$RESULTS_FILE"
        fi
    fi
    
    echo "" | tee -a "$RESULTS_FILE"
done

echo "================================================" | tee -a "$RESULTS_FILE"
echo "Test run complete. Results saved to $RESULTS_FILE" | tee -a "$RESULTS_FILE"

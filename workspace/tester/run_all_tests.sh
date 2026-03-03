#!/bin/bash

REPO_ROOT="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014"
BASIC_DIR="$REPO_ROOT/testcases/basic-testcases"
BIGINT_DIR="$REPO_ROOT/testcases/bigint-testcases"
CODE="$REPO_ROOT/code"
OUTPUT_FILE="comprehensive_test_results.txt"

echo "=== COMPREHENSIVE TEST EXECUTION ===" > $OUTPUT_FILE
echo "Started: $(date)" >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE

# Run basic tests 0-15
echo "=== BASIC TESTS (test0-15) ===" >> $OUTPUT_FILE
for i in {0..15}; do
    echo "" >> $OUTPUT_FILE
    echo "--- Test $i ---" >> $OUTPUT_FILE
    if [ -f "$BASIC_DIR/test$i.in" ]; then
        $CODE < "$BASIC_DIR/test$i.in" > "test${i}_output.txt" 2>&1
        exit_code=$?
        echo "Exit code: $exit_code" >> $OUTPUT_FILE
        
        if [ -f "$BASIC_DIR/test$i.out" ]; then
            if diff -q "test${i}_output.txt" "$BASIC_DIR/test$i.out" > /dev/null 2>&1; then
                echo "Result: PASS ✓" >> $OUTPUT_FILE
            else
                echo "Result: FAIL ✗" >> $OUTPUT_FILE
                echo "Expected output:" >> $OUTPUT_FILE
                head -20 "$BASIC_DIR/test$i.out" >> $OUTPUT_FILE
                echo "" >> $OUTPUT_FILE
                echo "Actual output:" >> $OUTPUT_FILE
                head -20 "test${i}_output.txt" >> $OUTPUT_FILE
            fi
        else
            echo "Result: NO EXPECTED OUTPUT FILE" >> $OUTPUT_FILE
            echo "Actual output:" >> $OUTPUT_FILE
            head -20 "test${i}_output.txt" >> $OUTPUT_FILE
        fi
    else
        echo "Test file not found" >> $OUTPUT_FILE
    fi
done

# Run BigInteger tests 0-19
echo "" >> $OUTPUT_FILE
echo "=== BIGINTEGER TESTS (BigIntegerTest0-19) ===" >> $OUTPUT_FILE
for i in {0..19}; do
    echo "" >> $OUTPUT_FILE
    echo "--- BigIntegerTest $i ---" >> $OUTPUT_FILE
    if [ -f "$BIGINT_DIR/BigIntegerTest$i.in" ]; then
        $CODE < "$BIGINT_DIR/BigIntegerTest$i.in" > "bigint${i}_output.txt" 2>&1
        exit_code=$?
        echo "Exit code: $exit_code" >> $OUTPUT_FILE
        
        if [ -f "$BIGINT_DIR/BigIntegerTest$i.out" ]; then
            if diff -q "bigint${i}_output.txt" "$BIGINT_DIR/BigIntegerTest$i.out" > /dev/null 2>&1; then
                echo "Result: PASS ✓" >> $OUTPUT_FILE
            else
                echo "Result: FAIL ✗" >> $OUTPUT_FILE
                echo "Expected output:" >> $OUTPUT_FILE
                head -20 "$BIGINT_DIR/BigIntegerTest$i.out" >> $OUTPUT_FILE
                echo "" >> $OUTPUT_FILE
                echo "Actual output:" >> $OUTPUT_FILE
                head -20 "bigint${i}_output.txt" >> $OUTPUT_FILE
            fi
        else
            echo "Result: NO EXPECTED OUTPUT FILE" >> $OUTPUT_FILE
            echo "Actual output:" >> $OUTPUT_FILE
            head -20 "bigint${i}_output.txt" >> $OUTPUT_FILE
        fi
    else
        echo "Test file not found" >> $OUTPUT_FILE
    fi
done

echo "" >> $OUTPUT_FILE
echo "=== TEST EXECUTION COMPLETED ===" >> $OUTPUT_FILE
echo "Ended: $(date)" >> $OUTPUT_FILE


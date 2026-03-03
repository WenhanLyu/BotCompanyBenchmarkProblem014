#!/bin/bash

REPO_ROOT="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014"
BASIC_DIR="$REPO_ROOT/testcases/basic-testcases"
CODE="$REPO_ROOT/code"

echo "=== BASIC TESTS 13-15 ===" > test13_15_results.txt

for i in 13 14 15; do
    echo "" >> test13_15_results.txt
    echo "--- Test $i ---" >> test13_15_results.txt
    if [ -f "$BASIC_DIR/test$i.in" ]; then
        echo "Running test $i..."
        $CODE < "$BASIC_DIR/test$i.in" > "test${i}_output.txt" 2>&1
        exit_code=$?
        echo "Exit code: $exit_code" >> test13_15_results.txt
        
        if [ -f "$BASIC_DIR/test$i.out" ]; then
            if diff -q "test${i}_output.txt" "$BASIC_DIR/test$i.out" > /dev/null 2>&1; then
                echo "Result: PASS ✓" >> test13_15_results.txt
            else
                echo "Result: FAIL ✗" >> test13_15_results.txt
                echo "Expected:" >> test13_15_results.txt
                cat "$BASIC_DIR/test$i.out" >> test13_15_results.txt
                echo "" >> test13_15_results.txt
                echo "Actual:" >> test13_15_results.txt
                cat "test${i}_output.txt" >> test13_15_results.txt
            fi
        else
            echo "Result: NO EXPECTED OUTPUT" >> test13_15_results.txt
            cat "test${i}_output.txt" >> test13_15_results.txt
        fi
    fi
done


#!/bin/bash
# Run all basic test cases to check for regressions

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

PASSED=0
FAILED=0

for i in {0..15}; do
    TEST_IN="testcases/basic-testcases/test${i}.in"
    TEST_OUT="testcases/basic-testcases/test${i}.out"
    
    if [ -f "$TEST_IN" ] && [ -f "$TEST_OUT" ]; then
        ACTUAL_OUT="/tmp/test${i}_actual.out"
        ./code < "$TEST_IN" > "$ACTUAL_OUT" 2>&1
        
        if diff -q "$TEST_OUT" "$ACTUAL_OUT" > /dev/null 2>&1; then
            echo "test${i}: PASS"
            PASSED=$((PASSED + 1))
        else
            echo "test${i}: FAIL"
            FAILED=$((FAILED + 1))
        fi
        rm -f "$ACTUAL_OUT"
    fi
done

echo ""
echo "Summary: $PASSED passed, $FAILED failed"

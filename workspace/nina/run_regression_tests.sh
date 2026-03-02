#!/bin/bash
# Run regression tests test0-test12

echo "Running regression tests (test0-test12)..."
echo ""

PASS=0
FAIL=0

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12; do
    TEST="test$i"
    INPUT="testcases/basic-testcases/$TEST.in"
    EXPECTED="testcases/basic-testcases/$TEST.out"
    
    if [ ! -f "$INPUT" ]; then
        echo "SKIP: $TEST (input not found)"
        continue
    fi
    
    if [ ! -f "$EXPECTED" ]; then
        echo "SKIP: $TEST (expected output not found)"
        continue
    fi
    
    # Run test with timeout
    /usr/bin/time -p ./code < "$INPUT" > "workspace/nina/${TEST}_reg.out" 2> "workspace/nina/${TEST}_reg.time"
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "FAIL: $TEST (exit code $EXIT_CODE)"
        FAIL=$((FAIL + 1))
        continue
    fi
    
    # Compare output
    if diff -q "$EXPECTED" "workspace/nina/${TEST}_reg.out" > /dev/null 2>&1; then
        ELAPSED=$(grep "^real" "workspace/nina/${TEST}_reg.time" | awk '{print $2}')
        echo "PASS: $TEST (${ELAPSED}s)"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $TEST (output mismatch)"
        FAIL=$((FAIL + 1))
    fi
done

echo ""
echo "========================================="
echo "REGRESSION TEST SUMMARY"
echo "========================================="
echo "PASS: $PASS"
echo "FAIL: $FAIL"
echo "========================================="

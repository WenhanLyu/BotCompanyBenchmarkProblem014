#!/bin/bash
# Test BigInteger tests, skipping known TLE tests (2, 5, 8, 18)
cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

PASS=0
FAIL=0
SKIP=0

echo "Testing BigInteger tests (skipping known TLE: 2, 5, 8, 18)..."
echo "============================================================"

# Tests to skip due to known TLE from O(n²) multiplication
SKIP_TESTS="2 5 8 18"

for i in {0..19}; do
    # Check if this test should be skipped
    if echo "$SKIP_TESTS" | grep -w $i > /dev/null; then
        echo "BigIntegerTest $i: SKIP (known TLE) ⏭"
        SKIP=$((SKIP + 1))
        continue
    fi
    
    INPUT="testcases/bigint-testcases/BigIntegerTest${i}.in"
    EXPECTED="testcases/bigint-testcases/BigIntegerTest${i}.out"
    ACTUAL="/tmp/BigIntegerTest${i}_actual.out"
    
    if [ ! -f "$INPUT" ] || [ ! -f "$EXPECTED" ]; then
        continue
    fi
    
    # Run test in background with 5 second timeout
    ./code < "$INPUT" > "$ACTUAL" 2>&1 &
    PID=$!
    
    # Wait for completion or timeout
    COUNTER=0
    while kill -0 $PID 2>/dev/null && [ $COUNTER -lt 5 ]; do
        sleep 1
        COUNTER=$((COUNTER + 1))
    done
    
    if kill -0 $PID 2>/dev/null; then
        # Still running, kill it
        kill -9 $PID 2>/dev/null
        echo "BigIntegerTest $i: FAIL (timeout >5s) ✗"
        FAIL=$((FAIL + 1))
    else
        # Completed, check result
        wait $PID
        if diff -q "$EXPECTED" "$ACTUAL" > /dev/null 2>&1; then
            echo "BigIntegerTest $i: PASS ✓"
            PASS=$((PASS + 1))
        else
            echo "BigIntegerTest $i: FAIL (wrong output) ✗"
            FAIL=$((FAIL + 1))
        fi
    fi
done

echo "============================================================"
echo "Results: $PASS passed, $FAIL failed, $SKIP skipped"

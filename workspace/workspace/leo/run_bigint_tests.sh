#!/bin/bash
# Run BigInteger tests individually with timeouts

passed=0
failed=0
timeout_limit=30  # 30 seconds per test

for i in {0..19}; do
    echo "Testing BigIntegerTest$i..."
    
    # Run with timeout using a background process
    ./code < testcases/bigint-testcases/BigIntegerTest$i.in > /tmp/bigint_test_$i.out 2>&1 &
    pid=$!
    
    # Wait for process with timeout
    count=0
    while kill -0 $pid 2>/dev/null && [ $count -lt $timeout_limit ]; do
        sleep 1
        ((count++))
    done
    
    # If still running, kill it
    if kill -0 $pid 2>/dev/null; then
        kill -9 $pid 2>/dev/null
        echo "  TIMEOUT (>$timeout_limit seconds)"
        ((failed++))
    else
        # Check if output matches
        if diff -q /tmp/bigint_test_$i.out testcases/bigint-testcases/BigIntegerTest$i.out > /dev/null 2>&1; then
            echo "  PASS"
            ((passed++))
        else
            echo "  FAIL (wrong output)"
            ((failed++))
        fi
    fi
done

echo ""
echo "====================================="
echo "Summary: $passed passed, $failed failed out of 20 tests"
echo "====================================="

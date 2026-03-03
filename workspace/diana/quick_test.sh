#!/bin/bash
# Quick test runner - tests one at a time

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
    INPUT="testcases/bigint-testcases/BigIntegerTest${i}.in"
    EXPECTED="testcases/bigint-testcases/BigIntegerTest${i}.out"
    
    if [ ! -f "$INPUT" ] || [ ! -f "$EXPECTED" ]; then
        echo "Test $i: SKIP (missing files)"
        continue
    fi
    
    # Run and capture output (no timeout on macOS)
    ACTUAL=$(./code < "$INPUT" 2>&1)
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "Test $i: ERROR (exit $EXIT_CODE)"
        continue
    fi
    
    # Compare
    EXPECTED_TEXT=$(cat "$EXPECTED")
    if [ "$ACTUAL" = "$EXPECTED_TEXT" ]; then
        echo "Test $i: PASS ✓"
    else
        # Count lines to see difference magnitude
        EXP_LINES=$(echo "$EXPECTED_TEXT" | wc -l | tr -d ' ')
        ACT_LINES=$(echo "$ACTUAL" | wc -l | tr -d ' ')
        echo "Test $i: FAIL (expected $EXP_LINES lines, got $ACT_LINES lines)"
    fi
done

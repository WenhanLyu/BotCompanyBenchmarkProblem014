#!/bin/bash
# Test a single BigInteger test

if [ -z "$1" ]; then
    echo "Usage: $0 <test_number>"
    exit 1
fi

TEST_NUM=$1
INPUT="testcases/bigint-testcases/BigIntegerTest${TEST_NUM}.in"
EXPECTED="testcases/bigint-testcases/BigIntegerTest${TEST_NUM}.out"

if [ ! -f "$INPUT" ] || [ ! -f "$EXPECTED" ]; then
    echo "Test files not found"
    exit 1
fi

echo "Running Test $TEST_NUM..."
ACTUAL=$(./code < "$INPUT" 2>&1)
EXIT_CODE=$?

if [ $EXIT_CODE -ne 0 ]; then
    echo "ERROR: Exit code $EXIT_CODE"
    echo "Output: $ACTUAL"
    exit 1
fi

EXPECTED_TEXT=$(cat "$EXPECTED")

if [ "$ACTUAL" = "$EXPECTED_TEXT" ]; then
    echo "PASS"
    exit 0
else
    echo "FAIL"
    echo ""
    echo "Expected output has $(echo "$EXPECTED_TEXT" | wc -l) lines"
    echo "Actual output has $(echo "$ACTUAL" | wc -l) lines"
    echo ""
    echo "First 5 lines of expected:"
    echo "$EXPECTED_TEXT" | head -5
    echo ""
    echo "First 5 lines of actual:"
    echo "$ACTUAL" | head -5
    echo ""
    echo "Last 5 lines of expected:"
    echo "$EXPECTED_TEXT" | tail -5
    echo ""
    echo "Last 5 lines of actual:"
    echo "$ACTUAL" | tail -5
    exit 1
fi

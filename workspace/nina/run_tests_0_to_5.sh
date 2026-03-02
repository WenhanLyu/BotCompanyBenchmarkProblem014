#!/bin/bash
# Test runner for tests 0-5 to validate M3 implementation

echo "==========================================="
echo "M3 Validation: Testing tests 0-5"
echo "==========================================="

PASS_COUNT=0
FAIL_COUNT=0

for i in {0..5}; do
    TEST="test$i"
    INPUT="testcases/basic-testcases/$TEST.in"
    EXPECTED="testcases/basic-testcases/$TEST.out"
    
    if [ ! -f "$INPUT" ]; then
        echo "  $TEST... SKIP (no input file)"
        continue
    fi
    
    if [ ! -f "$EXPECTED" ]; then
        echo "  $TEST... SKIP (no expected output)"
        continue
    fi
    
    # Run the test
    OUTPUT=$(./code < "$INPUT" 2>&1)
    EXPECTED_OUTPUT=$(cat "$EXPECTED")
    
    if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
        echo "  $TEST... PASS ✓"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "  $TEST... FAIL ✗"
        echo "    Expected:"
        echo "$EXPECTED_OUTPUT" | head -5
        echo "    Got:"
        echo "$OUTPUT" | head -5
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
done

echo "==========================================="
echo "Tests passed: $PASS_COUNT"
echo "Tests failed: $FAIL_COUNT"
echo "==========================================="

if [ $FAIL_COUNT -eq 0 ]; then
    echo "✓ All tests 0-5 passed!"
    exit 0
else
    echo "✗ Some tests failed"
    exit 1
fi

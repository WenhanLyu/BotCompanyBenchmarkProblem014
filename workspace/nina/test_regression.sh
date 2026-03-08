#!/bin/bash
cd /Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo

echo "=== Testing test0-12 for Regressions ==="
echo ""

PASS=0
FAIL=0

for i in {0..12}; do
    TEST="test$i"
    INPUT="testcases/basic-testcases/${TEST}.in"
    EXPECTED="testcases/basic-testcases/${TEST}.out"
    
    if [ ! -f "$INPUT" ]; then
        echo "⚠ $TEST: Input file not found, skipping"
        continue
    fi
    
    if [ ! -f "$EXPECTED" ]; then
        echo "⚠ $TEST: Expected output not found, skipping"
        continue
    fi
    
    # Run test
    cat "$INPUT" | ./code /dev/stdin > /tmp/${TEST}_actual.out 2>&1
    
    # Compare
    if diff -q "$EXPECTED" /tmp/${TEST}_actual.out > /dev/null 2>&1; then
        echo "✓ $TEST PASSED"
        ((PASS++))
    else
        echo "✗ $TEST FAILED"
        echo "  Expected:"
        head -5 "$EXPECTED"
        echo "  Actual:"
        head -5 /tmp/${TEST}_actual.out
        ((FAIL++))
    fi
done

echo ""
echo "=== Summary ==="
echo "Passed: $PASS"
echo "Failed: $FAIL"

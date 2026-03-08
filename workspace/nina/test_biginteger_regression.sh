#!/bin/bash
# BigInteger regression test script
# Tests BigIntegerTest 0-19

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

PASS=0
FAIL=0
TOTAL=0

echo "Running BigInteger regression tests (0-19)..."
echo "=============================================="

for i in {0..19}; do
    TOTAL=$((TOTAL + 1))
    INPUT="testcases/bigint-testcases/BigIntegerTest${i}.in"
    EXPECTED="testcases/bigint-testcases/BigIntegerTest${i}.out"
    ACTUAL="/tmp/BigIntegerTest${i}_actual.out"
    
    if [ ! -f "$INPUT" ]; then
        echo "Test $i: SKIP (no input file)"
        TOTAL=$((TOTAL - 1))
        continue
    fi
    
    if [ ! -f "$EXPECTED" ]; then
        echo "Test $i: SKIP (no expected output)"
        TOTAL=$((TOTAL - 1))
        continue
    fi
    
    # Run the test
    if ./code < "$INPUT" > "$ACTUAL" 2>&1; then
        # Compare outputs
        if diff -q "$EXPECTED" "$ACTUAL" > /dev/null 2>&1; then
            echo "BigIntegerTest $i: PASS ✓"
            PASS=$((PASS + 1))
        else
            echo "BigIntegerTest $i: FAIL (wrong output) ✗"
            FAIL=$((FAIL + 1))
            # Show first difference
            echo "  First difference:"
            diff "$EXPECTED" "$ACTUAL" | head -10 | sed 's/^/    /'
        fi
    else
        EXIT_CODE=$?
        echo "BigIntegerTest $i: FAIL (runtime error, exit code: $EXIT_CODE) ✗"
        FAIL=$((FAIL + 1))
    fi
done

echo "=============================================="
echo "Results: $PASS/$TOTAL passed, $FAIL failed"
echo ""

if [ $FAIL -eq 0 ]; then
    echo "✅ All BigInteger tests passed!"
    exit 0
else
    echo "❌ Some BigInteger tests failed"
    exit 1
fi

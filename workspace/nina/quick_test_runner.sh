#!/bin/bash
# Quick test runner for BigInteger tests

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "Testing BigIntegerTest0-19..."
for i in {0..19}; do
    echo -n "Test $i: "
    ./code < testcases/bigint-testcases/BigIntegerTest${i}.in > workspace/nina/BigIntegerTest${i}_actual.out 2>&1
    
    if diff -q testcases/bigint-testcases/BigIntegerTest${i}.out workspace/nina/BigIntegerTest${i}_actual.out > /dev/null 2>&1; then
        echo "✅ PASS"
    else
        echo "❌ FAIL"
        # Show first difference
        echo "  First diff:"
        diff testcases/bigint-testcases/BigIntegerTest${i}.out workspace/nina/BigIntegerTest${i}_actual.out | head -10 | sed 's/^/    /'
    fi
done

echo ""
echo "Testing regression (test0-12)..."
for i in 0 1 2 3 4 5 6 7 8 9 10 12; do
    echo -n "test$i: "
    ./code < testcases/basic-testcases/test${i}.in > workspace/nina/test${i}_check.out 2>&1
    
    if diff -q testcases/basic-testcases/test${i}.out workspace/nina/test${i}_check.out > /dev/null 2>&1; then
        echo "✅ PASS"
    else
        echo "❌ FAIL"
    fi
done

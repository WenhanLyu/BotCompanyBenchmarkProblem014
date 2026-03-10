#!/bin/bash
# Run all return statement tests

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "================================"
echo "Return Statement Tests"
echo "================================"
echo ""

PASS=0
FAIL=0

# Core tests
for test in test_return_nested_loops test_return_if_loop test_return_deep_nest test_return_early_exit; do
    cat workspace/jasper/${test}.in | ./code > workspace/jasper/${test}.out 2>&1
    if diff -q workspace/jasper/${test}.out workspace/jasper/${test}.expected > /dev/null 2>&1; then
        echo "✅ PASS: $test"
        PASS=$((PASS + 1))
    else
        echo "❌ FAIL: $test"
        FAIL=$((FAIL + 1))
    fi
done

echo ""
echo "================================"
echo "Results: $PASS passed, $FAIL failed"
echo "================================"

#!/bin/bash
# Quick test script for Issue #57
# Tests BigIntegerTest0 and regression tests

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "=== Testing BigIntegerTest0 ==="
time cat testcases/bigint-testcases/BigIntegerTest0.in | ./code /dev/stdin > /tmp/bigint0_result.out 2>&1
if diff -q testcases/bigint-testcases/BigIntegerTest0.out /tmp/bigint0_result.out > /dev/null 2>&1; then
    echo "BigIntegerTest0: PASS ✅"
else
    echo "BigIntegerTest0: FAIL ❌"
    echo "Running diff..."
    diff testcases/bigint-testcases/BigIntegerTest0.out /tmp/bigint0_result.out | head -20
fi

echo ""
echo "=== Testing regression tests (test0-test12) ==="
passed=0
failed=0
for i in 0 1 2 3 4 5 6 7 8 9 10 11 12; do
  cat testcases/basic-testcases/test$i.in | ./code /dev/stdin > /tmp/test$i.out 2>&1
  if diff -q testcases/basic-testcases/test$i.out /tmp/test$i.out > /dev/null 2>&1; then
    ((passed++))
  else
    echo "test$i: FAIL"
    ((failed++))
  fi
done

echo "Regression tests: $passed/13 passing"
if [ $failed -eq 0 ]; then
    echo "All regression tests PASS ✅"
else
    echo "$failed regression tests FAIL ❌"
fi

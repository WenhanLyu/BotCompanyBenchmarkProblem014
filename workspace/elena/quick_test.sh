#!/bin/bash
# Quick test script to check test status

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "=== Basic Tests 0-15 ==="
for i in {0..15}; do
    if [ -f "testcases/basic-testcases/test$i.in" ]; then
        # Use perl's alarm to timeout after 2 seconds
        perl -e 'alarm shift; exec @ARGV' 2 ./code < testcases/basic-testcases/test$i.in > /tmp/test$i.out 2>&1
        if [ $? -eq 142 ]; then
            echo "test$i: TIMEOUT"
        elif diff -q /tmp/test$i.out testcases/basic-testcases/test$i.out > /dev/null 2>&1; then
            echo "test$i: PASS"
        else
            echo "test$i: FAIL"
        fi
    fi
done

echo ""
echo "=== BigInteger Tests 0-9 ==="
for i in {0..9}; do
    if [ -f "testcases/bigint-testcases/BigIntegerTest$i.in" ]; then
        perl -e 'alarm shift; exec @ARGV' 2 ./code < testcases/bigint-testcases/BigIntegerTest$i.in > /tmp/bigint$i.out 2>&1
        if [ $? -eq 142 ]; then
            echo "BigIntTest$i: TIMEOUT"
        elif diff -q /tmp/bigint$i.out testcases/bigint-testcases/BigIntegerTest$i.out > /dev/null 2>&1; then
            echo "BigIntTest$i: PASS"
        else
            echo "BigIntTest$i: FAIL"
        fi
    fi
done

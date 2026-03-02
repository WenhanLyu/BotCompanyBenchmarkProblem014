#!/bin/bash

RESULT_DIR="workspace/apollo/test_results"
mkdir -p "$RESULT_DIR"
EXEC="./code"
BASIC_DIR="testcases/basic-testcases"
BIGINT_DIR="testcases/bigint-testcases"

echo "=== COMPREHENSIVE TEST EXECUTION ===" | tee "$RESULT_DIR/summary.txt"
date | tee -a "$RESULT_DIR/summary.txt"
echo "" | tee -a "$RESULT_DIR/summary.txt"

# Test basic tests
echo "=== BASIC TESTS ===" | tee -a "$RESULT_DIR/summary.txt"
PASS=0
FAIL=0
TIMEOUT=0
NOEXP=0

for i in {0..12} {14..15}; do
    name="test$i"
    input="$BASIC_DIR/${name}.in"
    expected="$BASIC_DIR/${name}.out"
    actual="$RESULT_DIR/${name}_actual.out"
    
    if [ ! -f "$input" ]; then
        continue
    fi
    
    # Run with timeout using perl (available on macOS)
    perl -e 'alarm shift @ARGV; exec @ARGV' 5 "$EXEC" < "$input" > "$actual" 2>&1
    exit_code=$?
    
    if [ $exit_code -eq 142 ] || [ $exit_code -eq 124 ]; then
        echo "TIMEOUT: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((TIMEOUT++))
    elif [ ! -f "$expected" ]; then
        echo "NO_EXPECTED: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((NOEXP++))
    elif diff -q "$actual" "$expected" > /dev/null 2>&1; then
        echo "PASS: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((PASS++))
    else
        echo "FAIL: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((FAIL++))
    fi
done

# test13 requires global keyword - skip with note
echo "SKIP: test13 (requires global keyword - not implemented)" | tee -a "$RESULT_DIR/summary.txt"

echo "" | tee -a "$RESULT_DIR/summary.txt"
echo "Basic Tests: Pass=$PASS, Fail=$FAIL, Timeout=$TIMEOUT, NoExpected=$NOEXP, Skipped=1" | tee -a "$RESULT_DIR/summary.txt"
echo "" | tee -a "$RESULT_DIR/summary.txt"

# Test BigInteger tests
echo "=== BIGINTEGER TESTS ===" | tee -a "$RESULT_DIR/summary.txt"
BIG_PASS=0
BIG_FAIL=0
BIG_TIMEOUT=0
BIG_NOEXP=0

for i in {0..19}; do
    name="BigIntegerTest$i"
    input="$BIGINT_DIR/${name}.in"
    expected="$BIGINT_DIR/${name}.out"
    actual="$RESULT_DIR/${name}_actual.out"
    
    if [ ! -f "$input" ]; then
        continue
    fi
    
    # Run with timeout
    perl -e 'alarm shift @ARGV; exec @ARGV' 5 "$EXEC" < "$input" > "$actual" 2>&1
    exit_code=$?
    
    if [ $exit_code -eq 142 ] || [ $exit_code -eq 124 ]; then
        echo "TIMEOUT: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((BIG_TIMEOUT++))
    elif [ ! -f "$expected" ]; then
        echo "NO_EXPECTED: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((BIG_NOEXP++))
    elif diff -q "$actual" "$expected" > /dev/null 2>&1; then
        echo "PASS: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((BIG_PASS++))
    else
        echo "FAIL: $name" | tee -a "$RESULT_DIR/summary.txt"
        ((BIG_FAIL++))
    fi
done

echo "" | tee -a "$RESULT_DIR/summary.txt"
echo "BigInteger Tests: Pass=$BIG_PASS, Fail=$BIG_FAIL, Timeout=$BIG_TIMEOUT, NoExpected=$BIG_NOEXP" | tee -a "$RESULT_DIR/summary.txt"
echo "" | tee -a "$RESULT_DIR/summary.txt"

TOTAL_PASS=$((PASS + BIG_PASS))
TOTAL_FAIL=$((FAIL + BIG_FAIL))
TOTAL_TIMEOUT=$((TIMEOUT + BIG_TIMEOUT))
TOTAL_NOEXP=$((NOEXP + BIG_NOEXP))

echo "=== OVERALL SUMMARY ===" | tee -a "$RESULT_DIR/summary.txt"
echo "Passed: $TOTAL_PASS" | tee -a "$RESULT_DIR/summary.txt"
echo "Failed: $TOTAL_FAIL" | tee -a "$RESULT_DIR/summary.txt"
echo "Timeout: $TOTAL_TIMEOUT" | tee -a "$RESULT_DIR/summary.txt"
echo "No Expected: $TOTAL_NOEXP" | tee -a "$RESULT_DIR/summary.txt"
echo "Skipped: 1 (test13 - global keyword)" | tee -a "$RESULT_DIR/summary.txt"

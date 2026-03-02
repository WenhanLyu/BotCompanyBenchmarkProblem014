#!/bin/bash

# Comprehensive test runner for all test cases
RESULT_DIR="workspace/apollo/test_results"
mkdir -p "$RESULT_DIR"

EXECUTABLE="./code"
BASIC_DIR="testcases/basic-testcases"
BIGINT_DIR="testcases/bigint-testcases"

echo "=== COMPREHENSIVE TEST EXECUTION ===" > "$RESULT_DIR/summary.txt"
echo "Timestamp: $(date)" >> "$RESULT_DIR/summary.txt"
echo "" >> "$RESULT_DIR/summary.txt"

# Function to run a single test
run_test() {
    local test_name=$1
    local input_file=$2
    local expected_file=$3
    local output_dir=$4
    
    echo "Running $test_name..."
    
    # Run the test
    if [ -f "$input_file" ]; then
        timeout 10 "$EXECUTABLE" < "$input_file" > "$output_dir/${test_name}_actual.out" 2>&1
        local exit_code=$?
        
        # Check if expected output exists
        if [ -f "$expected_file" ]; then
            # Compare outputs
            if diff -q "$output_dir/${test_name}_actual.out" "$expected_file" > /dev/null 2>&1; then
                echo "PASS: $test_name" | tee -a "$RESULT_DIR/summary.txt"
                return 0
            else
                echo "FAIL: $test_name (output mismatch)" | tee -a "$RESULT_DIR/summary.txt"
                echo "  Expected: $expected_file" >> "$RESULT_DIR/summary.txt"
                echo "  Actual: $output_dir/${test_name}_actual.out" >> "$RESULT_DIR/summary.txt"
                return 1
            fi
        else
            if [ $exit_code -eq 0 ]; then
                echo "UNKNOWN: $test_name (no expected output, exit code 0)" | tee -a "$RESULT_DIR/summary.txt"
            elif [ $exit_code -eq 124 ]; then
                echo "TIMEOUT: $test_name" | tee -a "$RESULT_DIR/summary.txt"
            else
                echo "ERROR: $test_name (exit code $exit_code)" | tee -a "$RESULT_DIR/summary.txt"
            fi
            return 2
        fi
    else
        echo "SKIP: $test_name (input file not found)" | tee -a "$RESULT_DIR/summary.txt"
        return 3
    fi
}

# Run basic tests (test0-test15)
echo "=== BASIC TESTS (test0-test15) ===" | tee -a "$RESULT_DIR/summary.txt"
BASIC_PASS=0
BASIC_FAIL=0
BASIC_UNKNOWN=0

for i in {0..15}; do
    test_name="test$i"
    input_file="$BASIC_DIR/${test_name}.in"
    expected_file="$BASIC_DIR/${test_name}.out"
    
    run_test "$test_name" "$input_file" "$expected_file" "$RESULT_DIR"
    result=$?
    
    if [ $result -eq 0 ]; then
        ((BASIC_PASS++))
    elif [ $result -eq 1 ]; then
        ((BASIC_FAIL++))
    else
        ((BASIC_UNKNOWN++))
    fi
done

echo "" | tee -a "$RESULT_DIR/summary.txt"
echo "Basic Tests Summary: Pass=$BASIC_PASS, Fail=$BASIC_FAIL, Unknown=$BASIC_UNKNOWN" | tee -a "$RESULT_DIR/summary.txt"
echo "" | tee -a "$RESULT_DIR/summary.txt"

# Run BigInteger tests (BigIntegerTest0-19)
echo "=== BIGINTEGER TESTS (BigIntegerTest0-19) ===" | tee -a "$RESULT_DIR/summary.txt"
BIGINT_PASS=0
BIGINT_FAIL=0
BIGINT_UNKNOWN=0

for i in {0..19}; do
    test_name="BigIntegerTest$i"
    input_file="$BIGINT_DIR/${test_name}.in"
    expected_file="$BIGINT_DIR/${test_name}.out"
    
    run_test "$test_name" "$input_file" "$expected_file" "$RESULT_DIR"
    result=$?
    
    if [ $result -eq 0 ]; then
        ((BIGINT_PASS++))
    elif [ $result -eq 1 ]; then
        ((BIGINT_FAIL++))
    else
        ((BIGINT_UNKNOWN++))
    fi
done

echo "" | tee -a "$RESULT_DIR/summary.txt"
echo "BigInteger Tests Summary: Pass=$BIGINT_PASS, Fail=$BIGINT_FAIL, Unknown=$BIGINT_UNKNOWN" | tee -a "$RESULT_DIR/summary.txt"
echo "" | tee -a "$RESULT_DIR/summary.txt"

# Overall summary
TOTAL_PASS=$((BASIC_PASS + BIGINT_PASS))
TOTAL_FAIL=$((BASIC_FAIL + BIGINT_FAIL))
TOTAL_UNKNOWN=$((BASIC_UNKNOWN + BIGINT_UNKNOWN))
TOTAL_TESTS=$((TOTAL_PASS + TOTAL_FAIL + TOTAL_UNKNOWN))

echo "=== OVERALL SUMMARY ===" | tee -a "$RESULT_DIR/summary.txt"
echo "Total Tests: $TOTAL_TESTS" | tee -a "$RESULT_DIR/summary.txt"
echo "Passed: $TOTAL_PASS" | tee -a "$RESULT_DIR/summary.txt"
echo "Failed: $TOTAL_FAIL" | tee -a "$RESULT_DIR/summary.txt"
echo "Unknown: $TOTAL_UNKNOWN" | tee -a "$RESULT_DIR/summary.txt"

echo ""
echo "Results saved to $RESULT_DIR/"
echo "Summary: $RESULT_DIR/summary.txt"

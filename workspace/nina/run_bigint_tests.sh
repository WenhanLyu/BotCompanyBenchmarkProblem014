#!/bin/bash
# Run all BigIntegerTest cases and check for regressions

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "=========================================="
echo "BigInteger Integration Test Suite"
echo "=========================================="
echo ""

# Counters
PASS=0
FAIL=0
CRASH=0

# Function to run a test
run_test() {
    local test_name=$1
    local input_file=$2
    local expected_file=$3
    local actual_file="workspace/nina/${test_name}_actual.out"
    
    echo -n "Testing ${test_name}... "
    
    # Run the test
    if ./code < "$input_file" > "$actual_file" 2>&1; then
        # Compare output
        if diff -q "$expected_file" "$actual_file" > /dev/null 2>&1; then
            echo "✅ PASS"
            PASS=$((PASS + 1))
            return 0
        else
            echo "❌ FAIL (output mismatch)"
            FAIL=$((FAIL + 1))
            return 1
        fi
    else
        local exit_code=$?
        echo "💥 CRASH (exit code: $exit_code)"
        CRASH=$((CRASH + 1))
        return 2
    fi
}

# Run BigInteger tests 0-19
echo "BigInteger Tests (0-19):"
echo "========================"
for i in {0..19}; do
    run_test "BigIntegerTest${i}" \
             "testcases/bigint-testcases/BigIntegerTest${i}.in" \
             "testcases/bigint-testcases/BigIntegerTest${i}.out"
done

echo ""
echo "=========================================="
echo "Regression Tests (test0-12):"
echo "=========================================="

# Run regression tests
for i in 0 1 2 3 4 5 6 7 8 9 10 12; do
    run_test "test${i}" \
             "testcases/basic-testcases/test${i}.in" \
             "testcases/basic-testcases/test${i}.out"
done

echo ""
echo "=========================================="
echo "Summary:"
echo "=========================================="
echo "BigInteger Tests: 20 total"
echo "Regression Tests: 12 total (test0-10, test12)"
echo "Total Tests: 32"
echo ""
echo "✅ PASS:    $PASS"
echo "❌ FAIL:    $FAIL"
echo "💥 CRASH:   $CRASH"
echo ""

if [ $FAIL -eq 0 ] && [ $CRASH -eq 0 ]; then
    echo "🎉 ALL TESTS PASSED!"
    exit 0
else
    echo "⚠️  SOME TESTS FAILED"
    exit 1
fi

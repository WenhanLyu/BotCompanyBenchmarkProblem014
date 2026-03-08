#!/bin/bash

# Comprehensive test script for all 20 BigIntegerTest cases using stdin method
# Issue #46 - Testing with correct stdin method (cat file | ./code /dev/stdin)

REPO_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo"
TESTCASE_DIR="$REPO_DIR/testcases/bigint-testcases"
OUTPUT_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/nina"

cd "$REPO_DIR" || exit 1

# Check if executable exists
if [ ! -f "./code" ]; then
    echo "ERROR: ./code executable not found in $REPO_DIR"
    exit 1
fi

# Initialize counters
TOTAL=20
PASSED=0
FAILED=0

# Arrays to track results
PASSING_TESTS=()
FAILING_TESTS=()

echo "=========================================="
echo "BigInteger Test Suite - All 20 Tests"
echo "Using STDIN method (correct approach)"
echo "=========================================="
echo ""

# Test all 20 BigIntegerTest cases (0-19)
for i in {0..19}; do
    TEST_NAME="BigIntegerTest$i"
    INPUT_FILE="$TESTCASE_DIR/${TEST_NAME}.in"
    EXPECTED_FILE="$TESTCASE_DIR/${TEST_NAME}.out"
    ACTUAL_FILE="$OUTPUT_DIR/${TEST_NAME}_actual.out"
    
    echo -n "Testing $TEST_NAME... "
    
    # Check if test files exist
    if [ ! -f "$INPUT_FILE" ]; then
        echo "SKIP (input not found)"
        continue
    fi
    if [ ! -f "$EXPECTED_FILE" ]; then
        echo "SKIP (expected output not found)"
        continue
    fi
    
    # Run test with stdin method
    cat "$INPUT_FILE" | ./code /dev/stdin > "$ACTUAL_FILE" 2>&1
    
    # Check results
    if diff -q "$EXPECTED_FILE" "$ACTUAL_FILE" > /dev/null 2>&1; then
        echo "PASS ✅"
        PASSED=$((PASSED + 1))
        PASSING_TESTS+=("$TEST_NAME")
    else
        echo "FAIL ❌"
        FAILED=$((FAILED + 1))
        FAILING_TESTS+=("$TEST_NAME")
    fi
done

echo ""
echo "=========================================="
echo "Test Results Summary"
echo "=========================================="
echo "Total tests:   $TOTAL"
echo "Passed:        $PASSED"
echo "Failed:        $FAILED"
echo ""

if [ ${#PASSING_TESTS[@]} -gt 0 ]; then
    echo "Passing tests (${#PASSING_TESTS[@]}):"
    for test in "${PASSING_TESTS[@]}"; do
        echo "  ✅ $test"
    done
    echo ""
fi

if [ ${#FAILING_TESTS[@]} -gt 0 ]; then
    echo "Failing tests (${#FAILING_TESTS[@]}):"
    for test in "${FAILING_TESTS[@]}"; do
        echo "  ❌ $test"
    done
    echo ""
    
    # Show first few diffs for failed tests
    echo "=========================================="
    echo "Sample Diffs (first 3 failed tests)"
    echo "=========================================="
    count=0
    for test in "${FAILING_TESTS[@]}"; do
        if [ $count -ge 3 ]; then
            break
        fi
        EXPECTED_FILE="$TESTCASE_DIR/${test}.out"
        ACTUAL_FILE="$OUTPUT_DIR/${test}_actual.out"
        echo ""
        echo "--- Diff for $test ---"
        echo "Expected first 5 lines:"
        head -5 "$EXPECTED_FILE"
        echo ""
        echo "Actual first 5 lines:"
        head -5 "$ACTUAL_FILE"
        echo ""
        echo "Line count - Expected: $(wc -l < "$EXPECTED_FILE") | Actual: $(wc -l < "$ACTUAL_FILE")"
        echo ""
        count=$((count + 1))
    done
fi

# Exit code based on results
if [ $PASSED -eq $TOTAL ]; then
    echo "🎉 ALL TESTS PASSED!"
    exit 0
else
    echo "⚠️  $FAILED tests failed"
    exit 1
fi

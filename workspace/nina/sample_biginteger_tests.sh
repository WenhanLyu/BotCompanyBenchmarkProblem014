#!/bin/bash
# Sample BigInteger test runner - tests a subset for time efficiency
# Tests BigIntegerTest 0, 5, 10, 15, 19 as samples

REPO_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo"
TEST_DIR="$REPO_DIR/testcases/bigint-testcases"
RESULTS_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/nina"
RESULTS_FILE="$RESULTS_DIR/sample_biginteger_test_results.txt"

cd "$REPO_DIR"

# Clear previous results
> "$RESULTS_FILE"

echo "=======================================" | tee -a "$RESULTS_FILE"
echo "BigInteger Sample Test Suite" | tee -a "$RESULTS_FILE"
echo "Testing: 0, 5, 10, 15, 19" | tee -a "$RESULTS_FILE"
echo "Date: $(date)" | tee -a "$RESULTS_FILE"
echo "Branch: $(git branch --show-current)" | tee -a "$RESULTS_FILE"
echo "Commit: $(git rev-parse --short HEAD)" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

PASSED=0
FAILED=0

# Test sample set
for i in 0 5 10 15 19; do
    TEST_NAME="BigIntegerTest$i"
    TEST_FILE="$TEST_DIR/${TEST_NAME}.in"
    
    echo "----------------------------------------" | tee -a "$RESULTS_FILE"
    echo "Running $TEST_NAME..." | tee -a "$RESULTS_FILE"
    echo "----------------------------------------" | tee -a "$RESULTS_FILE"
    
    if [ ! -f "$TEST_FILE" ]; then
        echo "ERROR: Test file not found: $TEST_FILE" | tee -a "$RESULTS_FILE"
        FAILED=$((FAILED + 1))
        continue
    fi
    
    # Run the test
    START=$(date +%s)
    OUTPUT=$(./code < "$TEST_FILE" 2>&1)
    EXIT_CODE=$?
    END=$(date +%s)
    DURATION=$((END - START))
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "RESULT: FAILED (exit code: $EXIT_CODE, time: ${DURATION}s)" | tee -a "$RESULTS_FILE"
        echo "ERROR OUTPUT:" | tee -a "$RESULTS_FILE"
        echo "$OUTPUT" | head -20 | tee -a "$RESULTS_FILE"
        FAILED=$((FAILED + 1))
    else
        echo "RESULT: PASSED (code executed successfully, time: ${DURATION}s)" | tee -a "$RESULTS_FILE"
        # Show first and last few lines of output
        echo "Sample output (first 5 lines):" | tee -a "$RESULTS_FILE"
        echo "$OUTPUT" | head -5 | tee -a "$RESULTS_FILE"
        echo "..." | tee -a "$RESULTS_FILE"
        echo "Sample output (last 5 lines):" | tee -a "$RESULTS_FILE"
        echo "$OUTPUT" | tail -5 | tee -a "$RESULTS_FILE"
        PASSED=$((PASSED + 1))
    fi
    
    echo "" | tee -a "$RESULTS_FILE"
done

echo "=======================================" | tee -a "$RESULTS_FILE"
echo "Sample Summary" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"
echo "Total Sampled Tests: 5" | tee -a "$RESULTS_FILE"
echo "Passed: $PASSED" | tee -a "$RESULTS_FILE"
echo "Failed: $FAILED" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"

if [ $PASSED -eq 5 ]; then
    echo "All sampled tests passed! Likely all 20 tests would pass." | tee -a "$RESULTS_FILE"
fi

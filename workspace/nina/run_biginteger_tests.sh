#!/bin/bash
# Comprehensive BigInteger test runner
# Tests all 20 BigIntegerTest cases

REPO_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo"
TEST_DIR="$REPO_DIR/testcases/bigint-testcases"
RESULTS_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/nina"
RESULTS_FILE="$RESULTS_DIR/biginteger_test_results.txt"

cd "$REPO_DIR"

# Clear previous results
> "$RESULTS_FILE"

echo "=======================================" | tee -a "$RESULTS_FILE"
echo "BigInteger Test Suite - Full Run" | tee -a "$RESULTS_FILE"
echo "Date: $(date)" | tee -a "$RESULTS_FILE"
echo "Branch: $(git branch --show-current)" | tee -a "$RESULTS_FILE"
echo "Commit: $(git rev-parse --short HEAD)" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

PASSED=0
FAILED=0
CRASHED=0

# Test all 20 BigIntegerTest cases
for i in {0..19}; do
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
    
    # Run the test (no timeout on macOS)
    OUTPUT=$(./code < "$TEST_FILE" 2>&1)
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "RESULT: FAILED (exit code: $EXIT_CODE)" | tee -a "$RESULTS_FILE"
        echo "ERROR OUTPUT:" | tee -a "$RESULTS_FILE"
        echo "$OUTPUT" | tee -a "$RESULTS_FILE"
        FAILED=$((FAILED + 1))
    else
        echo "RESULT: PASSED (code executed successfully)" | tee -a "$RESULTS_FILE"
        # Show first few lines of output
        echo "Sample output (first 10 lines):" | tee -a "$RESULTS_FILE"
        echo "$OUTPUT" | head -10 | tee -a "$RESULTS_FILE"
        PASSED=$((PASSED + 1))
    fi
    
    echo "" | tee -a "$RESULTS_FILE"
done

echo "=======================================" | tee -a "$RESULTS_FILE"
echo "Summary" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"
echo "Total Tests: 20" | tee -a "$RESULTS_FILE"
echo "Passed: $PASSED" | tee -a "$RESULTS_FILE"
echo "Failed: $FAILED" | tee -a "$RESULTS_FILE"
echo "Crashed/Timeout: $CRASHED" | tee -a "$RESULTS_FILE"
echo "=======================================" | tee -a "$RESULTS_FILE"

# Exit with appropriate code
if [ $PASSED -eq 20 ]; then
    exit 0
else
    exit 1
fi

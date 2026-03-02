#!/bin/bash
# Comprehensive BigInteger test with proper timeout handling

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

results_file="workspace/nina/bigint_test_results.txt"
> "$results_file"  # Clear file

echo "BigInteger Integration Tests - Comprehensive Report" | tee -a "$results_file"
echo "====================================================" | tee -a "$results_file"
echo "" | tee -a "$results_file"

PASS=0
FAIL=0
TIMEOUT=0
CRASH=0

test_with_timeout() {
    local num=$1
    local test_file="testcases/bigint-testcases/BigIntegerTest${num}.in"
    local expected_file="testcases/bigint-testcases/BigIntegerTest${num}.out"
    local actual_file="workspace/nina/BigIntegerTest${num}_actual.out"
    
    echo -n "BigIntegerTest${num}: " | tee -a "$results_file"
    
    # Run with background process and manual timeout
    ./code < "$test_file" > "$actual_file" 2>&1 &
    local pid=$!
    
    # Wait up to 15 seconds
    local count=0
    while kill -0 $pid 2>/dev/null && [ $count -lt 15 ]; do
        sleep 1
        count=$((count + 1))
    done
    
    # Check if still running
    if kill -0 $pid 2>/dev/null; then
        kill -9 $pid 2>/dev/null
        echo "⏱️  TIMEOUT (>15s)" | tee -a "$results_file"
        TIMEOUT=$((TIMEOUT + 1))
        return 2
    fi
    
    wait $pid
    local exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo "💥 CRASH (exit code: $exit_code)" | tee -a "$results_file"
        CRASH=$((CRASH + 1))
        return 2
    fi
    
    # Compare output
    if diff -q "$expected_file" "$actual_file" > /dev/null 2>&1; then
        echo "✅ PASS" | tee -a "$results_file"
        PASS=$((PASS + 1))
        return 0
    else
        echo "❌ FAIL" | tee -a "$results_file"
        # Save diff to a separate file
        diff "$expected_file" "$actual_file" > "workspace/nina/BigIntegerTest${num}_diff.txt" 2>&1
        echo "    Diff saved to: workspace/nina/BigIntegerTest${num}_diff.txt" | tee -a "$results_file"
        FAIL=$((FAIL + 1))
        return 1
    fi
}

# Test all 20 BigInteger tests
for i in {0..19}; do
    test_with_timeout $i
done

echo "" | tee -a "$results_file"
echo "====================================================" | tee -a "$results_file"
echo "Summary:" | tee -a "$results_file"
echo "  Total:    20" | tee -a "$results_file"
echo "  ✅ PASS:    $PASS" | tee -a "$results_file"
echo "  ❌ FAIL:    $FAIL" | tee -a "$results_file"
echo "  ⏱️  TIMEOUT: $TIMEOUT" | tee -a "$results_file"
echo "  💥 CRASH:   $CRASH" | tee -a "$results_file"
echo "====================================================" | tee -a "$results_file"

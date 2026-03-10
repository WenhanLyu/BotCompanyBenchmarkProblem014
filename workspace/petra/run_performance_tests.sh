#!/bin/bash

# Performance Test Runner for Large BigInteger Operations
# Measures execution time and verifies correctness

PROJECT_DIR="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014"
TESTDIR="$PROJECT_DIR/workspace/petra/large_tests"
INTERPRETER="$PROJECT_DIR/code"
RESULTS_FILE="$PROJECT_DIR/workspace/petra/performance_results.txt"

# Clear previous results
> "$RESULTS_FILE"

echo "================================" | tee -a "$RESULTS_FILE"
echo "Large Number Performance Tests" | tee -a "$RESULTS_FILE"
echo "Date: $(date)" | tee -a "$RESULTS_FILE"
echo "================================" | tee -a "$RESULTS_FILE"
echo "" | tee -a "$RESULTS_FILE"

# Function to run a test and measure time
run_perf_test() {
    local test_name=$1
    local description=$2
    
    echo "Test: $test_name - $description" | tee -a "$RESULTS_FILE"
    
    input_file="$TESTDIR/${test_name}.in"
    expected_file="$TESTDIR/${test_name}.out"
    
    # Use time command with output formatting
    # We'll run it multiple times to get more accurate timing
    
    total_time=0
    runs=3
    
    for run in $(seq 1 $runs); do
        start=$(python3 -c "import time; print(time.time())")
        actual_output=$("$INTERPRETER" < "$input_file" 2>&1)
        exit_code=$?
        end=$(python3 -c "import time; print(time.time())")
        
        elapsed=$(python3 -c "print($end - $start)")
        total_time=$(python3 -c "print($total_time + $elapsed)")
        
        if [ $run -eq 1 ]; then
            # Only check correctness on first run
            if [ $exit_code -ne 0 ]; then
                echo "  ERROR: Exit code $exit_code" | tee -a "$RESULTS_FILE"
                return 1
            fi
            
            expected_output=$(cat "$expected_file")
            if [ "$actual_output" != "$expected_output" ]; then
                echo "  ERROR: Output mismatch" | tee -a "$RESULTS_FILE"
                echo "  Expected: $expected_output" >> "$RESULTS_FILE"
                echo "  Actual: $actual_output" >> "$RESULTS_FILE"
                return 1
            fi
        fi
    done
    
    avg_time=$(python3 -c "print($total_time / $runs)")
    
    echo "  ✓ PASS" | tee -a "$RESULTS_FILE"
    echo "  Average time over $runs runs: ${avg_time}s" | tee -a "$RESULTS_FILE"
    echo "  Performance: $(python3 -c "print('GOOD (<1s)' if $avg_time < 1.0 else 'WARNING (>1s)')")" | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
    
    return 0
}

# Run all performance tests
run_perf_test "test1" "10^40 // 10^20 (simple division)"
run_perf_test "test2" "10^40 // 7 (worst case - large by small)"
run_perf_test "test3" "Similar magnitude large numbers"
run_perf_test "test4" "Division resulting in 0"
run_perf_test "test5" "10^50 // 10^25 (even larger numbers)"

echo "================================" | tee -a "$RESULTS_FILE"
echo "Performance Test Summary" | tee -a "$RESULTS_FILE"
echo "All tests completed" | tee -a "$RESULTS_FILE"
echo "================================" | tee -a "$RESULTS_FILE"

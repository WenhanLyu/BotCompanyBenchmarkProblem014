#!/bin/bash
# Test individual BigInteger cases with timeout handling

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

test_single() {
    local num=$1
    local test_file="testcases/bigint-testcases/BigIntegerTest${num}.in"
    local expected_file="testcases/bigint-testcases/BigIntegerTest${num}.out"
    local actual_file="workspace/nina/BigIntegerTest${num}_actual.out"
    
    echo "Testing BigIntegerTest${num}..."
    
    # Run with background process and manual timeout
    ./code < "$test_file" > "$actual_file" 2>&1 &
    local pid=$!
    
    # Wait up to 10 seconds
    local count=0
    while kill -0 $pid 2>/dev/null && [ $count -lt 10 ]; do
        sleep 1
        count=$((count + 1))
    done
    
    # Check if still running
    if kill -0 $pid 2>/dev/null; then
        kill -9 $pid 2>/dev/null
        echo "  ⏱️  TIMEOUT (>10s)"
        return 2
    fi
    
    wait $pid
    local exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo "  💥 CRASH (exit code: $exit_code)"
        return 2
    fi
    
    # Compare output
    if diff -q "$expected_file" "$actual_file" > /dev/null 2>&1; then
        echo "  ✅ PASS"
        return 0
    else
        echo "  ❌ FAIL (output mismatch)"
        echo "  Differences:"
        diff "$expected_file" "$actual_file" | head -15 | sed 's/^/    /'
        return 1
    fi
}

# Test first 5
for i in 0 1 2 3 4; do
    test_single $i
    echo ""
done

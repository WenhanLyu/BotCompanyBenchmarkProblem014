#!/bin/bash

REPO_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo"
TEST_DIR="$REPO_DIR/testcases/bigint-testcases"

echo "Testing BigIntegerTest cases..."
echo ""

pass_count=0
fail_count=0
crash_count=0

for i in {0..19}; do
    test_name="BigIntegerTest$i"
    input_file="$TEST_DIR/${test_name}.in"
    expected_file="$TEST_DIR/${test_name}.out"
    
    if [ ! -f "$input_file" ] || [ ! -f "$expected_file" ]; then
        echo "[$test_name] SKIP - Files not found"
        continue
    fi
    
    # Run test using stdin
    actual=$(cat "$input_file" | "$REPO_DIR/code" /dev/stdin 2>&1)
    exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo "[$test_name] CRASH (exit code $exit_code)"
        crash_count=$((crash_count + 1))
        continue
    fi
    
    expected=$(cat "$expected_file")
    
    if [ "$actual" = "$expected" ]; then
        echo "[$test_name] PASS"
        pass_count=$((pass_count + 1))
    else
        echo "[$test_name] FAIL - Wrong output"
        echo "  Expected lines: $(echo "$expected" | wc -l)"
        echo "  Actual lines: $(echo "$actual" | wc -l)"
        if [ -n "$actual" ]; then
            echo "  First line expected: $(echo "$expected" | head -1 | cut -c1-80)"
            echo "  First line actual: $(echo "$actual" | head -1 | cut -c1-80)"
        else
            echo "  Actual output: (empty)"
        fi
        fail_count=$((fail_count + 1))
    fi
done

echo ""
echo "========================================="
echo "Summary: $pass_count PASS, $fail_count FAIL, $crash_count CRASH"
echo "========================================="

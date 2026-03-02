#!/bin/bash
# Quick test runner for Nina

for i in 0 1 2 3 4 5 6 7 8 9; do
    test_file="testcases/basic-testcases/test${i}.in"
    expected="testcases/basic-testcases/test${i}.out"
    
    if [ -f "$test_file" ]; then
        echo -n "test${i}: "
        if [ -f "$expected" ]; then
            actual=$(./code < "$test_file" 2>&1)
            expected_content=$(cat "$expected")
            if [ "$actual" = "$expected_content" ]; then
                echo "✓ PASS"
            else
                echo "✗ FAIL"
            fi
        else
            echo "⚠ No expected output"
        fi
    fi
done

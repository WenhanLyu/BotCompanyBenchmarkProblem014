#!/bin/bash
# Comprehensive test verification script

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "=== Building project ==="
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ ! -f "./code" ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo "Build successful!"
echo ""

# Test all basic tests (0-15)
echo "=== Running Basic Tests (test0-test15) ==="
echo ""

passed=0
failed=0
total=0

for i in {0..15}; do
    test_file="testcases/basic-testcases/test$i.in"
    expected_file="testcases/basic-testcases/test$i.out"
    output_file="workspace/marcus/test${i}_actual.out"
    
    if [ ! -f "$test_file" ]; then
        continue
    fi
    
    total=$((total + 1))
    
    # Run test
    ./code < "$test_file" > "$output_file" 2>&1
    
    # Compare output
    if diff -q "$output_file" "$expected_file" > /dev/null 2>&1; then
        echo "✅ test$i: PASS"
        passed=$((passed + 1))
    else
        echo "❌ test$i: FAIL"
        failed=$((failed + 1))
        echo "   Expected output:"
        head -3 "$expected_file" | sed 's/^/     /'
        echo "   Actual output:"
        head -3 "$output_file" | sed 's/^/     /'
        echo ""
    fi
done

echo ""
echo "=== Summary ==="
echo "Passed: $passed/$total"
echo "Failed: $failed/$total"
echo "Pass rate: $(( passed * 100 / total ))%"

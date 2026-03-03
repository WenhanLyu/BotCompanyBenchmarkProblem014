#!/bin/bash
CODE="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014/code"
BASIC_DIR="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014/testcases/basic-testcases"

# Test basic 0-12 (known to pass)
echo "Testing basic 0-12..."
pass=0
for i in {0..12}; do
    $CODE < "$BASIC_DIR/test$i.in" > "test${i}.out" 2>&1
    if diff -q "test${i}.out" "$BASIC_DIR/test$i.out" > /dev/null 2>&1; then
        ((pass++))
    fi
done
echo "Basic 0-12: $pass/13 passed"

# Test 13 features (global keyword)
echo ""
echo "Test 13 requires: global keyword (hangs without it)"

# Test 14-15 features (f-strings)  
echo "Test 14 requires: f-string formatting"
echo "Test 15 requires: nested f-string formatting"


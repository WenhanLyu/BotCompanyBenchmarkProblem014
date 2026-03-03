#!/bin/bash
# Run edge case tests systematically

REPO_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/ares/repo"
TEST_DIR="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/mia/tests"
RESULTS_FILE="/Users/wenhanlyu/.thebotcompany/dev/src/github.com/WenhanLyu/BotCompanyBenchmarkProblem014/workspace/workspace/mia/edge_test_results.txt"

mkdir -p "$TEST_DIR"
cd "$REPO_DIR"

echo "Edge Case Test Results" > "$RESULTS_FILE"
echo "======================" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

run_test() {
    local name=$1
    local file=$2
    local description=$3
    
    echo "" >> "$RESULTS_FILE"
    echo "TEST: $name" >> "$RESULTS_FILE"
    echo "Description: $description" >> "$RESULTS_FILE"
    echo "---" >> "$RESULTS_FILE"
    
    # Run our interpreter
    OUTPUT=$(./code < "$file" 2>&1)
    EXIT_CODE=$?
    
    echo "Output: $OUTPUT" >> "$RESULTS_FILE"
    echo "Exit Code: $EXIT_CODE" >> "$RESULTS_FILE"
    
    # Also run with Python for reference
    PY_OUTPUT=$(python3 "$file" 2>&1)
    PY_EXIT=$?
    echo "Python Output: $PY_OUTPUT" >> "$RESULTS_FILE"
    echo "Python Exit: $PY_EXIT" >> "$RESULTS_FILE"
    
    # Determine result
    if [ $EXIT_CODE -eq 134 ] || [ $EXIT_CODE -eq 139 ]; then
        echo "RESULT: CRASH (SIGABRT/SIGSEGV)" >> "$RESULTS_FILE"
    elif echo "$OUTPUT" | grep -q "bad_variant_access"; then
        echo "RESULT: CRASH (bad_variant_access)" >> "$RESULTS_FILE"
    elif [ $EXIT_CODE -ne 0 ]; then
        echo "RESULT: RUNTIME ERROR" >> "$RESULTS_FILE"
    elif [ "$OUTPUT" != "$PY_OUTPUT" ]; then
        echo "RESULT: WRONG OUTPUT" >> "$RESULTS_FILE"
    else
        echo "RESULT: PASS" >> "$RESULTS_FILE"
    fi
    
    echo "" >> "$RESULTS_FILE"
}

echo "Creating test files..."

# Division by zero tests
cat > "$TEST_DIR/div_zero_1.py" << 'EOF'
print(10 // 0)
EOF

cat > "$TEST_DIR/div_zero_2.py" << 'EOF'
print(10 % 0)
EOF

cat > "$TEST_DIR/div_zero_3.py" << 'EOF'
x = 0
print(10 // x)
EOF

cat > "$TEST_DIR/div_zero_4.py" << 'EOF'
x = 99999999999999999999
print(x // 0)
EOF

# String operation tests
cat > "$TEST_DIR/string_mult_1.py" << 'EOF'
print("ab" * 3)
EOF

cat > "$TEST_DIR/string_mult_2.py" << 'EOF'
print("x" * 0)
EOF

cat > "$TEST_DIR/string_mult_3.py" << 'EOF'
print("x" * -1)
EOF

cat > "$TEST_DIR/string_mult_4.py" << 'EOF'
s = "hello"
print(s * 2)
EOF

# Type mismatch tests
cat > "$TEST_DIR/type_1.py" << 'EOF'
print("ab" * "cd")
EOF

cat > "$TEST_DIR/type_2.py" << 'EOF'
print(5 + "hello")
EOF

cat > "$TEST_DIR/type_3.py" << 'EOF'
print("hello" / 2)
EOF

cat > "$TEST_DIR/type_4.py" << 'EOF'
x = "test"
y = 10
print(x + y)
EOF

# Large number tests
cat > "$TEST_DIR/bigint_1.py" << 'EOF'
x = 99999999999999999999
print(x + 1)
EOF

cat > "$TEST_DIR/bigint_2.py" << 'EOF'
x = 99999999999999999999
y = 99999999999999999999
print(x * y)
EOF

cat > "$TEST_DIR/bigint_3.py" << 'EOF'
x = 99999999999999999999
print(x // 2)
EOF

cat > "$TEST_DIR/bigint_4.py" << 'EOF'
x = -99999999999999999999
print(x - 1)
EOF

# Edge cases that might crash
cat > "$TEST_DIR/edge_1.py" << 'EOF'
x = None
print(x + 1)
EOF

cat > "$TEST_DIR/edge_2.py" << 'EOF'
print(undefined_variable)
EOF

cat > "$TEST_DIR/edge_3.py" << 'EOF'
x = "5"
print(x / 2)
EOF

cat > "$TEST_DIR/edge_4.py" << 'EOF'
x = 10.5
y = 0
print(x / y)
EOF

echo "Running tests..."

# Run all tests
run_test "div_zero_1" "$TEST_DIR/div_zero_1.py" "Integer division by zero"
run_test "div_zero_2" "$TEST_DIR/div_zero_2.py" "Modulo by zero"
run_test "div_zero_3" "$TEST_DIR/div_zero_3.py" "Division by zero variable"
run_test "div_zero_4" "$TEST_DIR/div_zero_4.py" "BigInteger division by zero"

run_test "string_mult_1" "$TEST_DIR/string_mult_1.py" "String multiply by positive int"
run_test "string_mult_2" "$TEST_DIR/string_mult_2.py" "String multiply by zero"
run_test "string_mult_3" "$TEST_DIR/string_mult_3.py" "String multiply by negative"
run_test "string_mult_4" "$TEST_DIR/string_mult_4.py" "String multiply with variable"

run_test "type_1" "$TEST_DIR/type_1.py" "String multiply by string"
run_test "type_2" "$TEST_DIR/type_2.py" "Add int and string"
run_test "type_3" "$TEST_DIR/type_3.py" "Divide string by int"
run_test "type_4" "$TEST_DIR/type_4.py" "Add string and int variables"

run_test "bigint_1" "$TEST_DIR/bigint_1.py" "BigInteger addition"
run_test "bigint_2" "$TEST_DIR/bigint_2.py" "BigInteger multiplication"
run_test "bigint_3" "$TEST_DIR/bigint_3.py" "BigInteger division"
run_test "bigint_4" "$TEST_DIR/bigint_4.py" "Negative BigInteger subtraction"

run_test "edge_1" "$TEST_DIR/edge_1.py" "None + int"
run_test "edge_2" "$TEST_DIR/edge_2.py" "Undefined variable"
run_test "edge_3" "$TEST_DIR/edge_3.py" "String division"
run_test "edge_4" "$TEST_DIR/edge_4.py" "Float division by zero"

echo "Tests complete! Results written to $RESULTS_FILE"
cat "$RESULTS_FILE"

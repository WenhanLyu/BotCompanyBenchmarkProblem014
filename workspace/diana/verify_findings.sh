#!/bin/bash
# Verification script for string concatenation edge cases and regression tests

echo "=================================="
echo "STRING CONCATENATION TEST SUITE"
echo "=================================="
echo ""

# Change to repo root
cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "1. Running Regression Tests (test0-6, test8-10, test12)..."
echo "-----------------------------------------------------------"
PASS=0
FAIL=0

for i in 0 1 2 3 4 5 6 8 9 10 12; do
    ./code < testcases/basic-testcases/test$i.in > /tmp/test$i.out 2>&1
    if diff -q testcases/basic-testcases/test$i.out /tmp/test$i.out > /dev/null 2>&1; then
        echo "  test$i: PASS ✅"
        ((PASS++))
    else
        echo "  test$i: FAIL ❌"
        ((FAIL++))
    fi
done

echo ""
echo "Regression Test Summary: $PASS passed, $FAIL failed"
echo ""

echo "2. Testing Edge Cases..."
echo "-----------------------------------------------------------"

# Test 1: Empty string
echo -n "  Empty string concatenation: "
cat > /tmp/edge1.in << 'EOF'
empty = ""
print(empty + "test")
EOF
OUTPUT=$(./code < /tmp/edge1.in 2>&1)
if [ "$OUTPUT" = "test" ]; then
    echo "PASS ✅"
else
    echo "FAIL ❌"
fi

# Test 2: Multiple concatenation
echo -n "  Multiple concatenation: "
cat > /tmp/edge2.in << 'EOF'
print("A" + "B" + "C" + "D")
EOF
OUTPUT=$(./code < /tmp/edge2.in 2>&1)
if [ "$OUTPUT" = "ABCD" ]; then
    echo "PASS ✅"
else
    echo "FAIL ❌"
fi

# Test 3: Parenthesized (known bug)
echo -n "  Parenthesized concatenation: "
cat > /tmp/edge3.in << 'EOF'
a = "A"
b = "B"
c = (a + b)
print(c)
EOF
OUTPUT=$(./code < /tmp/edge3.in 2>&1)
if [ "$OUTPUT" = "AB" ]; then
    echo "PASS ✅"
else
    echo "FAIL ❌ (Known bug: returns '$OUTPUT' instead of 'AB')"
fi

# Test 4: Unicode
echo -n "  Unicode concatenation: "
cat > /tmp/edge4.in << 'EOF'
print("你好" + "世界")
EOF
OUTPUT=$(./code < /tmp/edge4.in 2>&1)
if [ "$OUTPUT" = "你好世界" ]; then
    echo "PASS ✅"
else
    echo "FAIL ❌"
fi

# Test 5: Augmented assignment
echo -n "  Augmented assignment (+=): "
cat > /tmp/edge5.in << 'EOF'
s = "Hello"
s += " World"
print(s)
EOF
OUTPUT=$(./code < /tmp/edge5.in 2>&1)
if [ "$OUTPUT" = "Hello World" ]; then
    echo "PASS ✅"
else
    echo "FAIL ❌"
fi

echo ""
echo "=================================="
echo "VERIFICATION COMPLETE"
echo "=================================="
echo ""
echo "See detailed reports in workspace/diana/:"
echo "  - edge_case_and_regression_report.md"
echo "  - regression_test_details.md"
echo "  - SUMMARY.md"

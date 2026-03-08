#!/bin/bash
# Test suite for global scope fix (Issue #82)

cd /Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014

echo "=== Global Scope Fix Test Suite ==="
echo ""

# Test 1: Basic global modification
echo "Test 1: Basic global modification"
cat > /tmp/test1.py << 'EOF'
x = 10
def modify():
    x += 5
    return x
print(modify())
print(x)
EOF
./code < /tmp/test1.py > /tmp/test1.out 2>&1
EXPECTED="15
15"
ACTUAL=$(cat /tmp/test1.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

# Test 2: Parameter shadowing
echo "Test 2: Parameter shadowing"
cat > /tmp/test2.py << 'EOF'
x = 100
def test(x):
    x = 200
    return x
print(test(50))
print(x)
EOF
./code < /tmp/test2.py > /tmp/test2.out 2>&1
EXPECTED="200
100"
ACTUAL=$(cat /tmp/test2.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

# Test 3: Multiple global modifications
echo "Test 3: Multiple global modifications"
cat > /tmp/test3.py << 'EOF'
a = 1
b = 2
def modify_both():
    a += 10
    b += 20
    return a + b
print(modify_both())
print(a)
print(b)
EOF
./code < /tmp/test3.py > /tmp/test3.out 2>&1
EXPECTED="33
11
22"
ACTUAL=$(cat /tmp/test3.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

# Test 4: Nested function calls with globals
echo "Test 4: Nested function calls with globals"
cat > /tmp/test4.py << 'EOF'
counter = 0
def increment():
    counter += 1
    return counter
def double_increment():
    increment()
    increment()
    return counter
print(double_increment())
print(counter)
EOF
./code < /tmp/test4.py > /tmp/test4.out 2>&1
EXPECTED="2
2"
ACTUAL=$(cat /tmp/test4.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

# Test 5: Parameter with same name as global
echo "Test 5: Parameter with same name as global"
cat > /tmp/test5.py << 'EOF'
x = 100
def test(x):
    print(x)
    x = 200
    print(x)
test(50)
print(x)
EOF
./code < /tmp/test5.py > /tmp/test5.out 2>&1
EXPECTED="50
200
100"
ACTUAL=$(cat /tmp/test5.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

# Test 6: Reading global without modification
echo "Test 6: Reading global without modification"
cat > /tmp/test6.py << 'EOF'
x = 100
def read_global():
    print(x)
read_global()
print(x)
EOF
./code < /tmp/test6.py > /tmp/test6.out 2>&1
EXPECTED="100
100"
ACTUAL=$(cat /tmp/test6.out)
if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "✓ PASS"
else
    echo "✗ FAIL"
    echo "Expected: $EXPECTED"
    echo "Actual: $ACTUAL"
fi
echo ""

echo "=== Test Suite Complete ==="

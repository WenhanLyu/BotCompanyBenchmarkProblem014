#!/bin/bash
# Test current implementation against critical edge cases
# Run from project root directory

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=========================================="
echo "Edge Case Testing for Current Implementation"
echo "=========================================="
echo ""

# Make sure code is built
if [ ! -f "./code" ]; then
    echo -e "${RED}Error: ./code not found. Run 'make' first.${NC}"
    exit 1
fi

PASS=0
FAIL=0
CRASH=0

# Helper function to run test
run_test() {
    local test_name="$1"
    local input="$2"
    local should_crash="$3"  # 1 if crash expected with current code, 0 if should work
    
    echo -n "Testing: $test_name ... "
    
    output=$(echo "$input" | timeout 2 ./code 2>&1)
    exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}TIMEOUT${NC}"
        FAIL=$((FAIL + 1))
    elif [ $exit_code -eq 134 ] || [ $exit_code -eq 139 ]; then
        if [ "$should_crash" = "1" ]; then
            echo -e "${YELLOW}CRASH (Expected - needs fix)${NC}"
            CRASH=$((CRASH + 1))
        else
            echo -e "${RED}CRASH (Unexpected)${NC}"
            FAIL=$((FAIL + 1))
        fi
    elif [ $exit_code -ne 0 ]; then
        echo -e "${YELLOW}ERROR (Exit $exit_code)${NC}"
        FAIL=$((FAIL + 1))
    else
        echo -e "${GREEN}PASS${NC}"
        PASS=$((PASS + 1))
    fi
}

echo "=== Basic Functionality Tests ==="
run_test "Simple print" "print(42)" 0
run_test "String print" "print('hello')" 0
run_test "Arithmetic" "print(10 + 20)" 0

echo ""
echo "=== Division by Zero Tests (Should NOT Crash) ==="
run_test "Floor div by zero" "print(10 // 0)" 0
run_test "Modulo by zero" "print(10 % 0)" 0
run_test "Float div by zero" "print(10.0 / 0.0)" 0
run_test "Variable div by zero" "x = 0\nprint(10 // x)" 0
run_test "Expression div by zero" "y = 5 - 5\nprint(100 // y)" 0

echo ""
echo "=== INT_MIN Negation Tests (May Crash - Needs Fix) ==="
run_test "INT_MIN negation" "x = -2147483648\ny = -x\nprint(y)" 1
run_test "Unary INT_MIN" "print(-(-2147483648))" 1

echo ""
echo "=== String Repetition Tests (Negative may crash) ==="
run_test "String * positive" "print('abc' * 3)" 0
run_test "String * zero" "print('abc' * 0)" 0
run_test "String * negative" "print('abc' * -1)" 1

echo ""
echo "=== Overflow Tests (Should auto-promote to BigInteger) ==="
run_test "Addition overflow" "x = 2147483647\nprint(x + 1)" 0
run_test "Subtraction overflow" "x = -2147483648\nprint(x - 1)" 0
run_test "Multiplication overflow" "x = 2147483647\nprint(x * 2)" 0

echo ""
echo "=== Type Safety Tests ==="
run_test "Int to string coercion" "x = 5\ny = 'test'\nprint(x + y)" 0
run_test "String concat" "print('hello' + ' world')" 0

echo ""
echo "=========================================="
echo "Results:"
echo -e "  ${GREEN}PASS:${NC}  $PASS"
echo -e "  ${YELLOW}CRASH (Expected - needs fix):${NC} $CRASH"
echo -e "  ${RED}FAIL:${NC}  $FAIL"
echo ""

if [ $CRASH -gt 0 ]; then
    echo -e "${YELLOW}WARNING: $CRASH test(s) crashed as expected.${NC}"
    echo "These are known issues that need fixing:"
    echo "  - INT_MIN negation overflow"
    echo "  - Negative string repetition"
    echo ""
    echo "Apply fixes from: workspace/isaac/defensive_fixes_quick_reference.md"
fi

if [ $FAIL -gt 0 ]; then
    echo -e "${RED}ERROR: $FAIL test(s) failed unexpectedly.${NC}"
    exit 1
fi

if [ $CRASH -eq 0 ] && [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}All tests passed! Code is robust.${NC}"
    exit 0
fi

exit 0

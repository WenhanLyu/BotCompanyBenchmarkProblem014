#!/bin/bash
# M1 Acceptance Test Script
# Tests: test0 (comments only) and test1 (simple print)
# 
# Usage: ./test_m1.sh
# Returns: 0 if all tests pass, 1 if any test fails

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TESTS_PASSED=0
TESTS_FAILED=0

echo "========================================="
echo "M1 Acceptance Tests"
echo "========================================="
echo ""

# Step 1: Build the project
echo -e "${YELLOW}[1/4] Building project...${NC}"
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}ERROR: CMakeLists.txt not found${NC}"
    exit 1
fi

# Clean previous build
rm -f code CMakeCache.txt
rm -rf CMakeFiles

# Run cmake
if ! cmake . > /dev/null 2>&1; then
    echo -e "${RED}ERROR: cmake failed${NC}"
    echo "Run 'cmake .' manually to see errors"
    exit 1
fi

# Run make
if ! make > /dev/null 2>&1; then
    echo -e "${RED}ERROR: make failed${NC}"
    echo "Run 'make' manually to see errors"
    exit 1
fi

# Check if executable exists
if [ ! -f "code" ]; then
    echo -e "${RED}ERROR: Executable 'code' not found after build${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build successful${NC}"
echo ""

# Step 2: Run tests
echo -e "${YELLOW}[2/4] Running tests...${NC}"

# Function to run a single test
run_test() {
    local test_name=$1
    local test_input="testcases/basic-testcases/${test_name}.in"
    local expected_output="testcases/basic-testcases/${test_name}.out"
    local actual_output="/tmp/${test_name}_actual.out"
    
    echo -n "  Testing ${test_name}... "
    
    # Check if input file exists
    if [ ! -f "$test_input" ]; then
        echo -e "${RED}FAIL (input file not found)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Check if expected output exists
    if [ ! -f "$expected_output" ]; then
        echo -e "${RED}FAIL (expected output not found)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Run the interpreter
    # Use timeout if available (Linux), gtimeout on macOS (if installed), otherwise run without timeout
    if command -v timeout > /dev/null 2>&1; then
        TIMEOUT_CMD="timeout 5"
    elif command -v gtimeout > /dev/null 2>&1; then
        TIMEOUT_CMD="gtimeout 5"
    else
        TIMEOUT_CMD=""
    fi
    
    if ! $TIMEOUT_CMD ./code < "$test_input" > "$actual_output" 2>&1; then
        echo -e "${RED}FAIL (runtime error or timeout)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Compare outputs
    if diff -q "$expected_output" "$actual_output" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        rm -f "$actual_output"
        return 0
    else
        echo -e "${RED}FAIL (output mismatch)${NC}"
        echo "    Expected:"
        cat "$expected_output" | sed 's/^/      /'
        echo "    Got:"
        cat "$actual_output" | sed 's/^/      /'
        echo "    Diff:"
        diff "$expected_output" "$actual_output" | sed 's/^/      /' || true
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$actual_output"
        return 1
    fi
}

# Run M1 tests
run_test "test0"
run_test "test1"

echo ""

# Step 3: Check for memory leaks (optional, requires valgrind)
echo -e "${YELLOW}[3/4] Checking for memory leaks...${NC}"
if command -v valgrind > /dev/null 2>&1; then
    echo "  Running valgrind on test1..."
    VALGRIND_OUTPUT="/tmp/valgrind_test1.log"
    
    # Determine timeout command
    if command -v timeout > /dev/null 2>&1; then
        VALGRIND_TIMEOUT="timeout 30"
    elif command -v gtimeout > /dev/null 2>&1; then
        VALGRIND_TIMEOUT="gtimeout 30"
    else
        VALGRIND_TIMEOUT=""
    fi
    
    if $VALGRIND_TIMEOUT valgrind --leak-check=full --error-exitcode=1 \
        ./code < testcases/basic-testcases/test1.in > /dev/null 2> "$VALGRIND_OUTPUT"; then
        echo -e "${GREEN}✓ No memory leaks detected${NC}"
    else
        echo -e "${YELLOW}⚠ Memory leaks detected or valgrind error${NC}"
        echo "  See $VALGRIND_OUTPUT for details"
    fi
else
    echo -e "${YELLOW}  valgrind not installed, skipping memory leak check${NC}"
    echo "  To install: sudo apt install valgrind"
fi

echo ""

# Step 4: Summary
echo -e "${YELLOW}[4/4] Test Summary${NC}"
echo "========================================="
echo -e "Tests passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Tests failed: ${RED}${TESTS_FAILED}${NC}"
echo "========================================="
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All M1 acceptance tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed${NC}"
    exit 1
fi

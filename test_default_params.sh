#!/bin/bash
# Test script for default parameters
# Tests comprehensive default parameter functionality

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "========================================="
echo "Default Parameters Test Suite"
echo "========================================="
echo ""

# Check if executable exists
if [ ! -f "./code" ]; then
    echo -e "${RED}ERROR: ./code executable not found${NC}"
    echo "Please build the project first with: cmake . && make"
    exit 1
fi

# Run the test
echo -e "${YELLOW}Running default parameters tests...${NC}"
./code < testcases/test_default_params.py > testcases/test_default_params_actual.out 2>&1

# Check exit code
if [ $? -ne 0 ]; then
    echo -e "${RED}FAILED: Tests failed to run${NC}"
    cat testcases/test_default_params_actual.out
    exit 1
fi

# Compare with expected output
if diff -q testcases/test_default_params.out testcases/test_default_params_actual.out > /dev/null 2>&1; then
    echo -e "${GREEN}PASSED: All default parameter tests passed${NC}"
    rm testcases/test_default_params_actual.out
    exit 0
else
    echo -e "${RED}FAILED: Output mismatch${NC}"
    echo ""
    echo "Expected output:"
    cat testcases/test_default_params.out
    echo ""
    echo "Actual output:"
    cat testcases/test_default_params_actual.out
    echo ""
    echo "Diff:"
    diff testcases/test_default_params.out testcases/test_default_params_actual.out || true
    exit 1
fi

#!/bin/bash
# Comprehensive Test Execution Script
# Tests: 16 basic tests (test0-test15) + 20 BigInteger tests (BigIntegerTest0-19)
# Total: 36 tests
#
# Usage: ./run_all_36_tests.sh
# Returns: 0 if all tests pass, 1 if any test fails

set -e

# Output directory
WORKSPACE_DIR="workspace/marcus"
REPORT="$WORKSPACE_DIR/test_execution_report.md"

# Colors for terminal output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
BASIC_PASS=0
BASIC_FAIL=0
BIGINT_PASS=0
BIGINT_FAIL=0

echo "========================================="
echo "COMPREHENSIVE TEST SUITE EXECUTION"
echo "36 Tests: 16 Basic + 20 BigInteger"
echo "========================================="
echo ""

# Initialize report
echo "# Comprehensive Test Execution Report" > "$REPORT"
echo "" >> "$REPORT"
echo "**Executor:** Marcus (Architecture Researcher)" >> "$REPORT"
echo "**Date:** $(date)" >> "$REPORT"
echo "**Repository:** tbc-pdb-014" >> "$REPORT"
echo "**Total Tests:** 36 (16 basic + 20 BigInteger)" >> "$REPORT"
echo "" >> "$REPORT"

# Check if executable exists
if [ ! -x ./code ]; then
    echo -e "${RED}ERROR: Executable './code' not found or not executable${NC}"
    echo "❌ **Build failed** - Interpreter not found" >> "$REPORT"
    exit 1
fi

echo -e "${GREEN}✓ Interpreter found at ./code${NC}"
echo "## Build Status" >> "$REPORT"
echo "" >> "$REPORT"
echo "✅ **Build successful** - Interpreter executable found at ./code" >> "$REPORT"
echo "" >> "$REPORT"

###########################################
# PART 1: BASIC TESTS (test0-test15)
###########################################

echo ""
echo -e "${YELLOW}[1/2] Running Basic Tests (test0-test15)...${NC}"
echo ""

echo "## Part 1: Basic Tests (test0-test15)" >> "$REPORT"
echo "" >> "$REPORT"

for i in {0..15}; do
    TEST_NAME="test$i"
    INPUT_FILE="testcases/basic-testcases/${TEST_NAME}.in"
    EXPECTED_FILE="testcases/basic-testcases/${TEST_NAME}.out"
    OUTPUT_FILE="$WORKSPACE_DIR/${TEST_NAME}_actual.out"
    
    echo -n "  Testing ${TEST_NAME}... "
    
    # Check if files exist
    if [ ! -f "$INPUT_FILE" ]; then
        echo -e "${RED}FAIL (input file not found)${NC}"
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Reason:** Input file not found" >> "$REPORT"
        echo "" >> "$REPORT"
        ((BASIC_FAIL++))
        continue
    fi
    
    if [ ! -f "$EXPECTED_FILE" ]; then
        echo -e "${RED}FAIL (expected output not found)${NC}"
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Reason:** Expected output file not found" >> "$REPORT"
        echo "" >> "$REPORT"
        ((BASIC_FAIL++))
        continue
    fi
    
    # Run the test with timeout (if available)
    START_TIME=$(date +%s.%N 2>/dev/null || date +%s)
    if command -v timeout > /dev/null 2>&1; then
        TIMEOUT_CMD="timeout 5"
    elif command -v gtimeout > /dev/null 2>&1; then
        TIMEOUT_CMD="gtimeout 5"
    else
        TIMEOUT_CMD=""
    fi
    
    if $TIMEOUT_CMD ./code < "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1; then
        EXIT_CODE=0
    else
        EXIT_CODE=$?
    fi
    END_TIME=$(date +%s.%N 2>/dev/null || date +%s)
    EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc 2>/dev/null || echo "N/A")
    
    # Compare outputs
    if diff -q "$EXPECTED_FILE" "$OUTPUT_FILE" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC} (${EXEC_TIME}s)"
        ((BASIC_PASS++))
        
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ✅ PASS" >> "$REPORT"
        echo "- **Execution Time:** ${EXEC_TIME}s" >> "$REPORT"
        echo "" >> "$REPORT"
    else
        echo -e "${RED}FAIL${NC}"
        ((BASIC_FAIL++))
        
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Execution Time:** ${EXEC_TIME}s" >> "$REPORT"
        echo "- **Exit Code:** $EXIT_CODE" >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Expected Output:**" >> "$REPORT"
        echo '```' >> "$REPORT"
        cat "$EXPECTED_FILE" >> "$REPORT"
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Actual Output:**" >> "$REPORT"
        echo '```' >> "$REPORT"
        cat "$OUTPUT_FILE" >> "$REPORT"
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Diff:**" >> "$REPORT"
        echo '```diff' >> "$REPORT"
        diff "$EXPECTED_FILE" "$OUTPUT_FILE" >> "$REPORT" || true
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
    fi
done

echo ""
echo "### Basic Tests Summary" >> "$REPORT"
echo "" >> "$REPORT"
echo "**Total:** 16 tests" >> "$REPORT"
echo "**Passed:** $BASIC_PASS" >> "$REPORT"
echo "**Failed:** $BASIC_FAIL" >> "$REPORT"
echo "**Pass Rate:** $(echo "scale=1; $BASIC_PASS * 100 / 16" | bc)%" >> "$REPORT"
echo "" >> "$REPORT"

###########################################
# PART 2: BIGINTEGER TESTS (BigIntegerTest0-19)
###########################################

echo ""
echo -e "${YELLOW}[2/2] Running BigInteger Tests (BigIntegerTest0-19)...${NC}"
echo ""

echo "## Part 2: BigInteger Tests (BigIntegerTest0-19)" >> "$REPORT"
echo "" >> "$REPORT"

for i in {0..19}; do
    TEST_NAME="BigIntegerTest$i"
    INPUT_FILE="testcases/bigint-testcases/${TEST_NAME}.in"
    EXPECTED_FILE="testcases/bigint-testcases/${TEST_NAME}.out"
    OUTPUT_FILE="$WORKSPACE_DIR/${TEST_NAME}_actual.out"
    
    echo -n "  Testing ${TEST_NAME}... "
    
    # Check if files exist
    if [ ! -f "$INPUT_FILE" ]; then
        echo -e "${RED}FAIL (input file not found)${NC}"
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Reason:** Input file not found" >> "$REPORT"
        echo "" >> "$REPORT"
        ((BIGINT_FAIL++))
        continue
    fi
    
    if [ ! -f "$EXPECTED_FILE" ]; then
        echo -e "${RED}FAIL (expected output not found)${NC}"
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Reason:** Expected output file not found" >> "$REPORT"
        echo "" >> "$REPORT"
        ((BIGINT_FAIL++))
        continue
    fi
    
    # Run the test with timeout (5 seconds max, if available)
    START_TIME=$(date +%s.%N 2>/dev/null || date +%s)
    if command -v timeout > /dev/null 2>&1; then
        TIMEOUT_CMD="timeout 5"
    elif command -v gtimeout > /dev/null 2>&1; then
        TIMEOUT_CMD="gtimeout 5"
    else
        TIMEOUT_CMD=""
    fi
    
    if $TIMEOUT_CMD ./code < "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1; then
        EXIT_CODE=0
    else
        EXIT_CODE=$?
    fi
    END_TIME=$(date +%s.%N 2>/dev/null || date +%s)
    EXEC_TIME=$(echo "$END_TIME - $START_TIME" | bc 2>/dev/null || echo "N/A")
    
    # Check for timeout (exit code 124 from timeout command)
    if [ $EXIT_CODE -eq 124 ]; then
        echo -e "${RED}FAIL (timeout)${NC}"
        ((BIGINT_FAIL++))
        
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL - TIMEOUT" >> "$REPORT"
        echo "- **Execution Time:** >5s (timeout)" >> "$REPORT"
        echo "- **Note:** Test exceeded 5-second time limit" >> "$REPORT"
        echo "" >> "$REPORT"
        continue
    fi
    
    # Compare outputs
    if diff -q "$EXPECTED_FILE" "$OUTPUT_FILE" > /dev/null 2>&1; then
        TIME_WARNING=""
        if (( $(echo "$EXEC_TIME > 1.0" | bc -l 2>/dev/null || echo "0") )); then
            TIME_WARNING=" ⚠️ (slow)"
        fi
        echo -e "${GREEN}PASS${NC}${TIME_WARNING} (${EXEC_TIME}s)"
        ((BIGINT_PASS++))
        
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ✅ PASS" >> "$REPORT"
        echo "- **Execution Time:** ${EXEC_TIME}s" >> "$REPORT"
        if (( $(echo "$EXEC_TIME > 1.0" | bc -l 2>/dev/null || echo "0") )); then
            echo "- **Note:** ⚠️ Execution time >1s (may be slow for OJ)" >> "$REPORT"
        fi
        echo "" >> "$REPORT"
    else
        echo -e "${RED}FAIL${NC}"
        ((BIGINT_FAIL++))
        
        echo "### $TEST_NAME" >> "$REPORT"
        echo "- **Status:** ❌ FAIL" >> "$REPORT"
        echo "- **Execution Time:** ${EXEC_TIME}s" >> "$REPORT"
        echo "- **Exit Code:** $EXIT_CODE" >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Expected Output:**" >> "$REPORT"
        echo '```' >> "$REPORT"
        head -100 "$EXPECTED_FILE" >> "$REPORT"
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Actual Output:**" >> "$REPORT"
        echo '```' >> "$REPORT"
        head -100 "$OUTPUT_FILE" >> "$REPORT"
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
        echo "**Diff (first 50 lines):**" >> "$REPORT"
        echo '```diff' >> "$REPORT"
        diff "$EXPECTED_FILE" "$OUTPUT_FILE" | head -50 >> "$REPORT" || true
        echo '```' >> "$REPORT"
        echo "" >> "$REPORT"
    fi
done

echo ""
echo "### BigInteger Tests Summary" >> "$REPORT"
echo "" >> "$REPORT"
echo "**Total:** 20 tests" >> "$REPORT"
echo "**Passed:** $BIGINT_PASS" >> "$REPORT"
echo "**Failed:** $BIGINT_FAIL" >> "$REPORT"
echo "**Pass Rate:** $(echo "scale=1; $BIGINT_PASS * 100 / 20" | bc)%" >> "$REPORT"
echo "" >> "$REPORT"

###########################################
# OVERALL SUMMARY
###########################################

TOTAL_PASS=$((BASIC_PASS + BIGINT_PASS))
TOTAL_FAIL=$((BASIC_FAIL + BIGINT_FAIL))
OVERALL_RATE=$(echo "scale=1; $TOTAL_PASS * 100 / 36" | bc)

echo ""
echo "========================================="
echo "OVERALL SUMMARY"
echo "========================================="
echo -e "Basic Tests:    ${GREEN}${BASIC_PASS}${NC}/${RED}${BASIC_FAIL}${NC} (16 total)"
echo -e "BigInt Tests:   ${GREEN}${BIGINT_PASS}${NC}/${RED}${BIGINT_FAIL}${NC} (20 total)"
echo -e "Total Passed:   ${GREEN}${TOTAL_PASS}${NC}/36 (${OVERALL_RATE}%)"
echo -e "Total Failed:   ${RED}${TOTAL_FAIL}${NC}/36"
echo "========================================="
echo ""

echo "## Overall Summary" >> "$REPORT"
echo "" >> "$REPORT"
echo "| Category | Passed | Failed | Total | Pass Rate |" >> "$REPORT"
echo "|----------|--------|--------|-------|-----------|" >> "$REPORT"
echo "| Basic Tests | $BASIC_PASS | $BASIC_FAIL | 16 | $(echo "scale=1; $BASIC_PASS * 100 / 16" | bc)% |" >> "$REPORT"
echo "| BigInteger Tests | $BIGINT_PASS | $BIGINT_FAIL | 20 | $(echo "scale=1; $BIGINT_PASS * 100 / 20" | bc)% |" >> "$REPORT"
echo "| **Total** | **$TOTAL_PASS** | **$TOTAL_FAIL** | **36** | **${OVERALL_RATE}%** |" >> "$REPORT"
echo "" >> "$REPORT"

if [ $TOTAL_FAIL -eq 0 ]; then
    echo -e "${GREEN}✓ ALL 36 TESTS PASSED! 🎉${NC}"
    echo "" >> "$REPORT"
    echo "## ✅ SUCCESS: 100% Pass Rate Achieved!" >> "$REPORT"
    echo "" >> "$REPORT"
    echo "All 36 regression tests passed successfully:" >> "$REPORT"
    echo "- ✅ All 16 basic tests (test0-test15) passing" >> "$REPORT"
    echo "- ✅ All 20 BigInteger tests (BigIntegerTest0-19) passing" >> "$REPORT"
    echo "- ✅ No timeouts or performance issues" >> "$REPORT"
    echo "" >> "$REPORT"
else
    echo -e "${RED}✗ Some tests failed (${TOTAL_FAIL}/36)${NC}"
    echo "" >> "$REPORT"
    echo "## ❌ FAILURE: Some Tests Did Not Pass" >> "$REPORT"
    echo "" >> "$REPORT"
    echo "$TOTAL_FAIL out of 36 tests failed. See detailed results above." >> "$REPORT"
    echo "" >> "$REPORT"
fi

echo "Report saved to: $REPORT"
echo ""

# Print report to stdout as well
cat "$REPORT"

# Exit with appropriate code
if [ $TOTAL_FAIL -eq 0 ]; then
    exit 0
else
    exit 1
fi

#!/bin/bash
# M10 Milestone Verification Test Suite
# Aria - QA Verification Engineer

echo "========================================="
echo "M10 MILESTONE VERIFICATION"
echo "========================================="
echo ""

# Test counters
PASS=0
FAIL=0
TOTAL=0

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "1. BASIC REGRESSION TESTS (test0-test15)"
echo "-----------------------------------------"
cd testcases/basic-testcases
for i in {0..15}; do
  if [ -f "test$i.in" ]; then
    TOTAL=$((TOTAL + 1))
    ../../code < test$i.in > test${i}_verify.out 2>&1
    if [ -f "test$i.out" ]; then
      if diff -q test$i.out test${i}_verify.out > /dev/null 2>&1; then
        echo -e "  test$i: ${GREEN}✅ PASS${NC}"
        PASS=$((PASS + 1))
      else
        echo -e "  test$i: ${RED}❌ FAIL${NC}"
        FAIL=$((FAIL + 1))
      fi
    else
      echo -e "  test$i: ${YELLOW}⚠️  SKIP (no expected output)${NC}"
      TOTAL=$((TOTAL - 1))
    fi
  fi
done
cd ../..
echo ""

echo "2. BREAK STATEMENT TESTS"
echo "-------------------------"
cd workspace/aria/tests

# Test break simple
echo 'i = 0
while i < 10:
    if i == 5:
        break
    print(i)
    i = i + 1
print("done")' > test_break.py
TOTAL=$((TOTAL + 1))
../../../code < test_break.py > break_out.txt 2>&1
python3 test_break.py > break_expected.txt 2>&1
if diff -q break_expected.txt break_out.txt > /dev/null 2>&1; then
  echo -e "  break simple: ${GREEN}✅ PASS${NC}"
  PASS=$((PASS + 1))
else
  echo -e "  break simple: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

# Test nested break
echo 'i = 0
while i < 3:
    j = 0
    while j < 5:
        if j == 2:
            break
        print(i, j)
        j = j + 1
    i = i + 1' > test_nested_break.py
TOTAL=$((TOTAL + 1))
../../../code < test_nested_break.py > nested_break_out.txt 2>&1
python3 test_nested_break.py > nested_break_expected.txt 2>&1
if diff -q nested_break_expected.txt nested_break_out.txt > /dev/null 2>&1; then
  echo -e "  break nested: ${GREEN}✅ PASS${NC}"
  PASS=$((PASS + 1))
else
  echo -e "  break nested: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

cd ../../..
echo ""

echo "3. CONTINUE STATEMENT TESTS"
echo "----------------------------"
cd workspace/aria/tests

# Test continue simple
echo 'i = 0
while i < 10:
    i = i + 1
    if i % 2 == 0:
        continue
    print(i)
print("done")' > test_continue.py
TOTAL=$((TOTAL + 1))
../../../code < test_continue.py > continue_out.txt 2>&1
python3 test_continue.py > continue_expected.txt 2>&1
if diff -q continue_expected.txt continue_out.txt > /dev/null 2>&1; then
  echo -e "  continue simple: ${GREEN}✅ PASS${NC}"
  PASS=$((PASS + 1))
else
  echo -e "  continue simple: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

cd ../../..
echo ""

echo "4. BIGINTEGER DIVISION PERFORMANCE"
echo "-----------------------------------"
cd workspace/aria/tests

# Test division performance
echo 'x = 123456789012345678901234567890123456789012345678901234567890
y = 987654321098765432109876543210
print(x // y)
print(x % y)' > test_big_div.py
TOTAL=$((TOTAL + 1))
START=$(date +%s.%N)
../../../code < test_big_div.py > big_div_out.txt 2>&1
END=$(date +%s.%N)
ELAPSED=$(echo "$END - $START" | bc)
python3 test_big_div.py > big_div_expected.txt 2>&1
if diff -q big_div_expected.txt big_div_out.txt > /dev/null 2>&1; then
  if (( $(echo "$ELAPSED < 1.0" | bc -l) )); then
    echo -e "  division performance: ${GREEN}✅ PASS${NC} (${ELAPSED}s < 1.0s)"
    PASS=$((PASS + 1))
  else
    echo -e "  division performance: ${YELLOW}⚠️  SLOW${NC} (${ELAPSED}s >= 1.0s)"
    FAIL=$((FAIL + 1))
  fi
else
  echo -e "  division correctness: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

cd ../../..
echo ""

echo "5. DEFENSIVE FIXES"
echo "------------------"
cd workspace/aria/tests

# Test INT_MIN negation
echo 'a = -2147483648
b = -a
print(b)' > test_int_min.py
TOTAL=$((TOTAL + 1))
../../../code < test_int_min.py > int_min_out.txt 2>&1
python3 test_int_min.py > int_min_expected.txt 2>&1
if diff -q int_min_expected.txt int_min_out.txt > /dev/null 2>&1; then
  echo -e "  INT_MIN negation: ${GREEN}✅ PASS${NC}"
  PASS=$((PASS + 1))
else
  echo -e "  INT_MIN negation: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

# Test negative string repetition
echo 's = "abc" * -5
print(s)
print("done")' > test_neg_str.py
TOTAL=$((TOTAL + 1))
../../../code < test_neg_str.py > neg_str_out.txt 2>&1
python3 test_neg_str.py > neg_str_expected.txt 2>&1
if diff -q neg_str_expected.txt neg_str_out.txt > /dev/null 2>&1; then
  echo -e "  negative string repetition: ${GREEN}✅ PASS${NC}"
  PASS=$((PASS + 1))
else
  echo -e "  negative string repetition: ${RED}❌ FAIL${NC}"
  FAIL=$((FAIL + 1))
fi

cd ../../..
echo ""

echo "========================================="
echo "VERIFICATION SUMMARY"
echo "========================================="
echo "Total Tests: $TOTAL"
echo -e "Passed: ${GREEN}$PASS${NC}"
echo -e "Failed: ${RED}$FAIL${NC}"
echo ""
if [ $FAIL -eq 0 ]; then
  echo -e "${GREEN}✅ ALL TESTS PASSED${NC}"
  echo "M10 Milestone: COMPLETE"
else
  echo -e "${RED}❌ SOME TESTS FAILED${NC}"
  echo "M10 Milestone: INCOMPLETE"
fi
echo "========================================="

#!/bin/bash
cd testcases/basic-testcases
echo "# Test Execution Results" > ../../workspace/thomas/all_tests_detailed.txt
echo "" >> ../../workspace/thomas/all_tests_detailed.txt

for test in test0 test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12; do
  echo "=== Testing $test ===" | tee -a ../../workspace/thomas/all_tests_detailed.txt
  echo "## Input:" >> ../../workspace/thomas/all_tests_detailed.txt
  cat ${test}.in >> ../../workspace/thomas/all_tests_detailed.txt
  echo "" >> ../../workspace/thomas/all_tests_detailed.txt
  
  echo "## Expected Output:" >> ../../workspace/thomas/all_tests_detailed.txt
  cat ${test}.out >> ../../workspace/thomas/all_tests_detailed.txt
  echo "" >> ../../workspace/thomas/all_tests_detailed.txt
  
  echo "## Actual Output:" >> ../../workspace/thomas/all_tests_detailed.txt
  ../../code < ${test}.in > ${test}_thomas.out 2>&1
  cat ${test}_thomas.out >> ../../workspace/thomas/all_tests_detailed.txt
  echo "" >> ../../workspace/thomas/all_tests_detailed.txt
  
  echo "## Exit Code: $?" >> ../../workspace/thomas/all_tests_detailed.txt
  
  # Compare outputs
  if diff -q ${test}.out ${test}_thomas.out > /dev/null 2>&1; then
    echo "## Result: ✅ PASS" | tee -a ../../workspace/thomas/all_tests_detailed.txt
  else
    echo "## Result: ✗ FAIL" | tee -a ../../workspace/thomas/all_tests_detailed.txt
    echo "## Diff:" >> ../../workspace/thomas/all_tests_detailed.txt
    diff -u ${test}.out ${test}_thomas.out >> ../../workspace/thomas/all_tests_detailed.txt
  fi
  echo "" >> ../../workspace/thomas/all_tests_detailed.txt
  echo "---" >> ../../workspace/thomas/all_tests_detailed.txt
  echo "" >> ../../workspace/thomas/all_tests_detailed.txt
done

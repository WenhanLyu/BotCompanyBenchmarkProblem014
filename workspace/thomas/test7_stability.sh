#!/bin/bash
echo "=== test7 Stability Test (5 runs) ===" > workspace/thomas/test7_stability.txt
for i in 1 2 3 4 5; do
  echo "" >> workspace/thomas/test7_stability.txt
  echo "## Run $i:" >> workspace/thomas/test7_stability.txt
  ./code < testcases/basic-testcases/test7.in > workspace/thomas/test7_run${i}.out 2>&1
  echo "Exit code: $?" >> workspace/thomas/test7_stability.txt
  cat workspace/thomas/test7_run${i}.out >> workspace/thomas/test7_stability.txt
  
  if diff -q testcases/basic-testcases/test7.out workspace/thomas/test7_run${i}.out > /dev/null 2>&1; then
    echo "Status: ✅ PASS" >> workspace/thomas/test7_stability.txt
  else
    echo "Status: ✗ FAIL" >> workspace/thomas/test7_stability.txt
  fi
done

# Verify all runs are identical
echo "" >> workspace/thomas/test7_stability.txt
echo "## Consistency Check:" >> workspace/thomas/test7_stability.txt
if cmp -s workspace/thomas/test7_run1.out workspace/thomas/test7_run2.out && \
   cmp -s workspace/thomas/test7_run1.out workspace/thomas/test7_run3.out && \
   cmp -s workspace/thomas/test7_run1.out workspace/thomas/test7_run4.out && \
   cmp -s workspace/thomas/test7_run1.out workspace/thomas/test7_run5.out; then
  echo "✅ All runs produce identical output (byte-by-byte)" >> workspace/thomas/test7_stability.txt
else
  echo "✗ Runs produce different outputs" >> workspace/thomas/test7_stability.txt
fi

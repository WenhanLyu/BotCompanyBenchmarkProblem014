#!/bin/bash

# Time Test 13 specifically (which has very large numbers)
PROJECT_DIR="/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014"
INTERPRETER="$PROJECT_DIR/code"
INPUT="$PROJECT_DIR/testcases/bigint-testcases/BigIntegerTest13.in"

echo "Timing BigIntegerTest13 (very large numbers - thousands of digits)..."
echo ""

for i in {1..5}; do
    start=$(python3 -c "import time; print(time.time())")
    output=$("$INTERPRETER" < "$INPUT" 2>&1)
    end=$(python3 -c "import time; print(time.time())")
    elapsed=$(python3 -c "print($end - $start)")
    echo "Run $i: ${elapsed}s"
done

#!/bin/bash
# Generate expected outputs for all test cases using Python 3
# This script creates .out files for all .in files that don't have them yet

set -e

echo "Generating expected outputs using Python 3..."
echo ""

# Counter
GENERATED=0
SKIPPED=0

# Function to generate output for a test
generate_output() {
    local input_file=$1
    local output_file="${input_file%.in}.out"
    
    if [ -f "$output_file" ]; then
        echo "  Skip: $output_file (already exists)"
        SKIPPED=$((SKIPPED + 1))
    else
        echo "  Generate: $output_file"
        if python3 "$input_file" > "$output_file" 2>&1; then
            GENERATED=$((GENERATED + 1))
        else
            echo "    WARNING: Python 3 failed on $input_file"
            rm -f "$output_file"
        fi
    fi
}

# Generate for basic tests
echo "Basic testcases:"
for input in testcases/basic-testcases/*.in; do
    if [ -f "$input" ]; then
        generate_output "$input"
    fi
done

echo ""

# Generate for bigint tests
echo "BigInt testcases:"
for input in testcases/bigint-testcases/*.in; do
    if [ -f "$input" ]; then
        generate_output "$input"
    fi
done

echo ""
echo "========================================="
echo "Generated: $GENERATED files"
echo "Skipped: $SKIPPED files (already existed)"
echo "========================================="
echo ""
echo "Note: You may want to commit the new .out files:"
echo "  git add testcases/**/*.out"
echo "  git commit -m 'Add expected outputs for tests'"

#!/usr/bin/env python3
import sys

# Increase the limit for integer string conversion
sys.set_int_max_str_digits(100000)

# Read and execute the test file
if len(sys.argv) != 2:
    print("Usage: python3 generate_bigint_expected.py <test_file.in>", file=sys.stderr)
    sys.exit(1)

with open(sys.argv[1], 'r') as f:
    code = f.read()
    exec(code)

#!/usr/bin/env python3
"""
Large Number Performance Test Generator
Creates test cases with 10^40 scale numbers to verify division performance
"""

import time

# Test Case 1: 10^40 // 10^20 (easy case - many digits)
test1_input = f"""
a = {10**40}
b = {10**20}
print(a // b)
print(a % b)
"""

# Test Case 2: Large number divided by small number (worst case for division)
test2_input = f"""
a = {10**40}
b = 7
print(a // b)
print(a % b)
"""

# Test Case 3: Two very large numbers of similar magnitude
test3_input = f"""
a = {10**40 + 12345678901234567890}
b = {10**39 + 98765432109876543210}
print(a // b)
print(a % b)
"""

# Test Case 4: Division that results in 0
test4_input = f"""
a = {10**20}
b = {10**40}
print(a // b)
print(a % b)
"""

# Test Case 5: Very large dividend, medium divisor
test5_input = f"""
a = {10**50}
b = {10**25}
print(a // b)
print(a % b)
"""

# Calculate expected results
test1_expected_div = str(10**40 // 10**20)
test1_expected_mod = str(10**40 % 10**20)

test2_expected_div = str(10**40 // 7)
test2_expected_mod = str(10**40 % 7)

test3_a = 10**40 + 12345678901234567890
test3_b = 10**39 + 98765432109876543210
test3_expected_div = str(test3_a // test3_b)
test3_expected_mod = str(test3_a % test3_b)

test4_expected_div = str(10**20 // 10**40)
test4_expected_mod = str(10**20 % 10**40)

test5_expected_div = str(10**50 // 10**25)
test5_expected_mod = str(10**50 % 10**25)

# Write test files
tests = [
    ("test1", test1_input, f"{test1_expected_div}\n{test1_expected_mod}\n"),
    ("test2", test2_input, f"{test2_expected_div}\n{test2_expected_mod}\n"),
    ("test3", test3_input, f"{test3_expected_div}\n{test3_expected_mod}\n"),
    ("test4", test4_input, f"{test4_expected_div}\n{test4_expected_mod}\n"),
    ("test5", test5_input, f"{test5_expected_div}\n{test5_expected_mod}\n"),
]

import os
os.makedirs("/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014/workspace/petra/large_tests", exist_ok=True)

for name, input_data, expected in tests:
    with open(f"/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014/workspace/petra/large_tests/{name}.in", "w") as f:
        f.write(input_data)
    with open(f"/Users/wenhanlyu/Documents/BotCompany/tbc_projdevbench_runs/tbc-pdb-014/workspace/petra/large_tests/{name}.out", "w") as f:
        f.write(expected)

print("Test files created successfully!")
print("\nTest descriptions:")
print("  test1: 10^40 // 10^20 (simple division)")
print("  test2: 10^40 // 7 (worst case - large number by small divisor)")
print("  test3: Similar magnitude large numbers")
print("  test4: Division resulting in 0")
print("  test5: 10^50 // 10^25 (even larger numbers)")

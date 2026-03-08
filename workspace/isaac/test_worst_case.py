# Test to demonstrate worst-case division behavior
# This creates a scenario where estimate will be BASE-1 but actual quotient is small

# In BigInteger's BASE system (BASE = 10^9), we want:
# 1. remainder.digits.size() > divisor.digits.size() + 1
# 2. But actual quotient digit is small

# Example: Large power of 10 divided by (large power of 10 - 1)
# This forces the estimate logic to set estimate = BASE-1
# But the actual quotient will be 1 with large remainder

# Test case 1: Pathological case
# 10^100 / (10^50 - 1)
# In BASE=10^9, this is ~12 digits / ~6 digits
# Will trigger the "> size + 1" condition and set estimate = 999999999
# But actual quotient digit depends on the values

a = 10**100
b = 10**50 - 1
print(f"Test: {a} // {b}")
print(f"Expected result: {a // b}")
print("This should cause many iterations if the algorithm is flawed")

# Test case 2: Another pathological case
a2 = 10**200
b2 = 10**100 - 1
print(f"\nTest 2: 10^200 // (10^100 - 1)")
print(f"Expected result: {a2 // b2}")

# These large divisions should expose the O(BASE) behavior

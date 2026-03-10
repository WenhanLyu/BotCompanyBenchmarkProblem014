# Test case where estimate might be 0 but answer is non-zero
# This happens when remainder and divisor have same digit count
# and remainder_lead < divisor_lead

# In BASE 10^9 system, let's construct:
# remainder = 500000000 (single digit, value 500000000)
# divisor = 600000000 (single digit, value 600000000)
# estimate = 500000000 // 600000000 = 0
# But this is correct! 500000000 / 600000000 = 0 remainder 500000000

# Let's try a case that actually divides evenly
# remainder = 600000000
# divisor = 600000000  
# estimate = 1, answer = 1

# Edge case: remainder slightly larger than divisor
a = 1000000001
b = 1000000000
print(f"a = {a}")
print(f"b = {b}")
print(f"print(a // b)")
print(f"print(a % b)")
print()

# Another edge case: Both have same leading digit but remainder is larger overall
# Due to lower order digits
# In decimal analogy: 19 / 10 = 1 remainder 9
# estimate from leading digit: 1/1 = 1 ✓

# Test case: 199 / 100 = 1 remainder 99
# estimate = 1 / 1 = 1 ✓
a = 199
b = 100
print(f"a = {a}")
print(f"b = {b}")
print(f"print(a // b)")
print(f"print(a % b)")
print()

# Test case: 299 / 100 = 2 remainder 99
# estimate = 2 / 1 = 2 ✓
a = 299
b = 100
print(f"a = {a}")
print(f"b = {b}")
print(f"print(a // b)")
print(f"print(a % b)")
print()

# Extreme: 999 / 100 = 9 remainder 99
# estimate = 9 / 1 = 9 ✓
a = 999
b = 100
print(f"a = {a}")
print(f"b = {b}")
print(f"print(a // b)")
print(f"print(a % b)")
print()

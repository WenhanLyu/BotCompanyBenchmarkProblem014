# Test case to demonstrate the algorithmic flaw in division
# This should trigger the worst-case scenario

# Case 1: Remainder is 2+ digits longer than divisor, but quotient digit is small
# This will set estimate = BASE-1 = 9999, but actual quotient digit might be 0-9

# Example: 100000 / 99999
# remainder starts at 100000 (6 digits in base 10, but let's think in terms of BigInteger's BASE=10000)
# divisor is 99999 (2 digits in BASE=10000: [9999, 9])
# The estimate logic will see remainder is longer and set estimate = 9999
# But actual quotient is 1, so it will iterate 9998 times!

print("10000000000 / 9999999999")
# This should take a long time with the flawed algorithm

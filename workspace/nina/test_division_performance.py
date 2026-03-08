# Test case for BigInteger division performance
# This tests the optimized division algorithm

# Test 1: Large division that should be fast with the new algorithm
x = 123456789012345678901234567890123456789012345678901234567890
y = 987654321098765432109876543210
print(x // y)
print(x % y)

# Test 2: Another large division
a = 999999999999999999999999999999999999999999999999999999999999
b = 123456789
print(a // b)
print(a % b)

# Test 3: Edge case with INT_MIN negation
print(-(-2147483648))

# Test 4: Negative string repetition
print("abc" * -5)
print("test" * 0)

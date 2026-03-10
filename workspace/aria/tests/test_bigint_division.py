# Test BigInteger division performance
# This was taking 19s before, should be <1s now

a = 10**100
b = 3
result = a // b
print(result)

# Test modulo as well
c = a % b
print(c)

# Test with larger numbers
x = 12345678901234567890123456789012345678901234567890
y = 987654321
z = x // y
print(z)
m = x % y
print(m)

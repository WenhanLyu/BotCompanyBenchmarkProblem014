# Test BigInteger division performance
# Large numbers that should complete quickly
a = 123456789012345678901234567890123456789012345678901234567890
b = 987654321098765432109876543210
result = a // b
print(result)
remainder = a % b
print(remainder)

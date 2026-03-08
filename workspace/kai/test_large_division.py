# Test case to reproduce Test 34 TLE issue
# This tests BigInteger division with large numbers

print("=== Testing Large BigInteger Division ===")

# Large number division - this should timeout with current O(n³) implementation
x = 123456789012345678901234567890123456789012345678901234567890
y = 987654321098765432109876543210

print("Test 1: Basic large division")
print(x // y)
print(x % y)

print("\nTest 2: Division by small number")
print(x // 123)

print("\nTest 3: Multiple divisions in loop")
for i in range(5):
    z = x // (y + i * 1000)
    print(z)

print("\nTest 4: Very large numbers")
big1 = 10**100
big2 = 10**50 + 7
print(big1 // big2)
print(big1 % big2)

print("\nTest 5: Edge case - division by 1")
print(x // 1)

print("\n=== All tests completed ===")

# Test edge cases from Part A defensive fixes

# Test 1: INT_MIN negation overflow protection
print("Test 1: INT_MIN negation")
x = -2147483648
print(-x)
print(type(-x))

# Test 2: Negative string repetition
print("Test 2: Negative string repetition")
print("abc" * -5)
print("test" * -1)
print("hello" * 0)

# Test 3: Positive cases still work
print("Test 3: Normal cases")
print("abc" * 3)
print(-100)
print(-(-100))

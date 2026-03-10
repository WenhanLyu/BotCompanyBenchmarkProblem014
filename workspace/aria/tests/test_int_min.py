# Test INT_MIN negation - should not crash
# INT_MIN in 32-bit is -2147483648
# In 64-bit it's -9223372036854775808
a = -2147483648
b = -a
print(b)

# Also test with 64-bit INT_MIN
c = -9223372036854775808
d = -c
print(d)

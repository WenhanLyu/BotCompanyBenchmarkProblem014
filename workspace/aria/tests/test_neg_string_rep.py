# Test negative string repetition - should return empty string, not crash
s = "abc" * -5
print(s)
print("done")

# Also test with 0
t = "xyz" * 0
print(t)
print("done2")

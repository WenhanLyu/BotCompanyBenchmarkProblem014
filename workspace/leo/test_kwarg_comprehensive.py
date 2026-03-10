# Comprehensive keyword argument tests

# Test 1: Basic keyword arguments
def f1(a, b, c):
    print(a, b, c)

print("Test 1: All positional")
f1(1, 2, 3)

print("Test 2: All keyword")
f1(c=30, a=10, b=20)

print("Test 3: Mixed - positional then keyword")
f1(1, c=3, b=2)

# Test 4: With default parameters
def f2(a, b=20, c=30):
    print(a, b, c)

print("Test 4: Only required positional")
f2(10)

print("Test 5: Keyword for optional param")
f2(10, c=300)

print("Test 6: Positional + keyword")
f2(10, 200, c=300)

print("Test 7: Keyword for all with default")
f2(c=3, a=1)

# Test 8: Multiple parameters with some defaults
def f3(x, y, z=100):
    print(x, y, z)

print("Test 8: Mix with defaults")
f3(1, 2)
f3(1, 2, 3)
f3(1, z=99, y=2)
f3(y=2, x=1, z=3)

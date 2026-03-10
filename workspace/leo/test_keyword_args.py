# Test keyword arguments
def f(a, b, c):
    print(a, b, c)

# All positional (should work)
f(1, 2, 3)

# All keyword (any order)
f(c=30, a=10, b=20)
f(b=2, a=1, c=3)

# Mixed (positional before keyword)
f(1, c=3, b=2)
f(1, 2, c=3)

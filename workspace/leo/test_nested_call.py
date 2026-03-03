counter = 0

def f(x):
    counter += 1
    print("f called with", x, "counter now", counter)
    return x + counter

# Test nested calls: a, b = f(1), f(f(2))
# Expected order:
# 1. f(1) -> counter=1, return 2
# 2. f(2) -> counter=2, return 4
# 3. f(4) -> counter=3, return 7
# Result: a=2, b=7

a, b = f(1), f(f(2))
print("Result:", a, b)

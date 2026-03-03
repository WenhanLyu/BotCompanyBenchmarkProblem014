counter = 0

def f(x):
    global counter
    counter += 1
    print("f called with", x, "counter now", counter)
    return x + counter

# Test nested calls: a, b = f(1), f(f(2))
a, b = f(1), f(f(2))
print("Result:", a, b)

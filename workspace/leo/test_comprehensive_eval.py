# Comprehensive evaluation order test
counter = 0

def inc():
    counter += 1
    print("Called", counter)
    return counter

# Test 1: Simple tuple
print("Test 1: Simple tuple")
a, b = inc(), inc()
print("Result:", a, b)

# Test 2: Nested expressions
print("Test 2: Nested expressions")
c = inc() + inc()
print("Result:", c)

# Test 3: More complex tuple
print("Test 3: More complex")
d, e, f = inc(), inc(), inc()
print("Result:", d, e, f)

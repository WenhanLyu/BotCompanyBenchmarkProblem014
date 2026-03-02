# Test multiple parameters
def add(a, b):
    print(a + b)

def multiply(x, y, z):
    print(x * y * z)

# Test 2 parameters
add(5, 3)
add(10, 20)

# Test 3 parameters
multiply(2, 3, 4)
multiply(5, 5, 5)

# Test local scope - parameters should not leak
def test_scope(p1, p2):
    x = p1 + p2
    print(x)

test_scope(100, 200)
# This should fail if p1, p2, or x leaked to outer scope
# (but we can't test undefined variable errors in this subset)

# Test parameter shadowing
a = 999
b = 888
add(1, 2)  # Should use parameters 1, 2, not outer a, b
print(a)  # Should still be 999
print(b)  # Should still be 888

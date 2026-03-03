# Test 1: Simple return
def foo():
    return 42

print(foo())

# Test 2: Return None (bare return)
def bar():
    return

print(bar())

# Test 3: No return statement (implicit None)
def baz():
    x = 1

print(baz())

# Test 4: Return expression
def calc(a, b):
    return a + b

print(calc(3, 4))

# Test 5: Return negative number
def neg():
    return -100

print(neg())

# Test 6: Return boolean
def truth():
    return True

print(truth())

# Test 7: Return string
def greet():
    return "Hello"

print(greet())

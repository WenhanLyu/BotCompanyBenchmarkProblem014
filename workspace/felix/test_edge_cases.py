# Edge case 1: Function with no parameters should still work
def no_params():
    print(42)

no_params()

# Edge case 2: Single parameter
def one_param(x):
    print(x * 2)

one_param(21)

# Edge case 3: Many parameters
def many_params(a, b, c, d, e):
    print(a + b + c + d + e)

many_params(1, 2, 3, 4, 5)

# Edge case 4: Nested function calls with parameters
def double(n):
    print(n * 2)

def quad(m):
    double(m)
    double(m)

quad(10)

# Edge case 5: Parameters of different types
def mixed_types(num, text):
    print(num)
    print(text)

mixed_types(123, "hello")

# Edge case 6: Parameter order matters
def subtract(a, b):
    print(a - b)

subtract(10, 3)
subtract(3, 10)

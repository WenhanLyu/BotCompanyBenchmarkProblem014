# Test error cases - each should be tested separately

def f(a, b, c):
    print(a, b, c)

# Error 1: Positional after keyword
# f(1, c=3, 2)  # Should error

# Error 2: Duplicate keyword
# f(a=1, b=2, a=3)  # Should error

# Error 3: Unknown parameter name
# f(a=1, b=2, d=3)  # Should error

# Error 4: Parameter specified multiple times
# f(1, a=2, b=3, c=4)  # Should error (a specified as both positional and keyword)

# These tests need to be run separately to see each error
print("Error tests - run each separately")

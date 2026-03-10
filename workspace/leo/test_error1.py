def f(a, b, c):
    print(a, b, c)

# Positional after keyword - should error
f(1, c=3, 2)

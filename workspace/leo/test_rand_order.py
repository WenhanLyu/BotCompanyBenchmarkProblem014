# Minimal test for rand() evaluation order
seed = 10

def rand():
    seed += 1
    return seed

# This should call rand() twice, left to right
a, b = rand(), rand()
print(a)  # Should be 11
print(b)  # Should be 12

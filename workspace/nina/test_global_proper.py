# Test proper global behavior - should fail without global keyword
seed = 10

def rand():
    # This creates a local variable in Python
    seed = seed + 1
    return seed

print(rand())

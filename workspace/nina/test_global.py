# Test global variable modification
seed = 10

def rand():
    seed += 1
    return seed

print(rand())
print(seed)

seed = 100

def rand():
    seed += 10
    return seed

print(rand())
print(seed)

seed = 1

def rand():
    global seed
    seed = seed + 1
    return seed

a, b = rand(), rand()
print(a, b)

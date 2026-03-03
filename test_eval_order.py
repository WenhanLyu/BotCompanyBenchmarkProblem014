seed = 1

def rand():
    global seed
    seed = seed + 1
    print(f"rand() called, seed={seed}")
    return seed

a, b = rand(), rand()
print(f"a={a}, b={b}")

seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

# Get first 10 random numbers
for i in range(10):
    r = rand()
    print(f"{i+1}: {r}")

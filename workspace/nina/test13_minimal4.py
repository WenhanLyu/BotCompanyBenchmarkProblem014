# Test with global seed
seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

print(random(100))
print(random(100))

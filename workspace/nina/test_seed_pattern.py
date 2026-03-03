# Test the exact pattern from test13

seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

print("First call:", random(100))
print("Second call:", random(100))
print("Third call:", random(100))
print("Final seed:", seed)

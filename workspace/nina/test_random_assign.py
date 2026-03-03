# Test the problematic line from test13
seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

# This line from test13: c , p = random(n - 1) + 1 , random(n - 1) + 1
n = 100
c, p = random(n - 1) + 1, random(n - 1) + 1
print(c, p)

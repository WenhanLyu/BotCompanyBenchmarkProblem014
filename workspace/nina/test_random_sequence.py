# Generate first 10 random numbers to compare with Python

seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

# Test with the first number from test13
n = 998244352
print("Random numbers for n =", n)
print("1:", random(n - 1) + 1)
print("2:", random(n - 1) + 1)
print("3:", random(n - 1) + 1)
print("4:", random(n - 1) + 1)
print("5:", random(n - 1) + 1)
print("6:", random(n - 1) + 1)
print("7:", random(n - 1) + 1)
print("8:", random(n - 1) + 1)
print("9:", random(n - 1) + 1)
print("10:", random(n - 1) + 1)

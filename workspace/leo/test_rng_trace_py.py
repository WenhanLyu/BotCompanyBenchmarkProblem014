seed = 19260817

def rand():
    global seed
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

# Get first 10 random numbers manually
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())
print(rand())

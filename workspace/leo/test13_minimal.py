# Minimal version of test13's rand() function
seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

# Test the sequence
print("First call:", rand())
print("Second call:", rand())
print("Third call:", rand())

# Reset
seed = 19260817
a, b = random(100), random(100)
print("Tuple call:", a, b)

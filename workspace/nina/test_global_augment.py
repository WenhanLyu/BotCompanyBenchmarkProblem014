# Test global with augmented assignment like test13
seed = 19260817

def rand():
    seed += seed * 131072
    return seed

print(rand())

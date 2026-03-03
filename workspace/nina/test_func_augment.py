# Test function with augmented assignment and return
def test(x, y):
    ret = 1
    ret *= x
    ret %= y
    return ret

result = test(5, 3)
print(result)

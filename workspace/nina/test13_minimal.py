# Minimal test13 reproduction
def quick_power(x, y, p):
    ret = 1
    while y != 0:
        if y % 2 == 1:
            ret *= x
            ret %= p
        x *= x
        x %= p
        y //= 2
    return ret

result = quick_power(2, 3, 10)
print(result)

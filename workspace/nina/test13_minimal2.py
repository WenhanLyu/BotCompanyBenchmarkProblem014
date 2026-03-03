# Test with quick_power and miller_rabin
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

def miller_rabin(x, n):
    cnt = 0
    m = n - 1
    while m % 2 == 0:
        cnt += 1
        m //= 2
    x = quick_power(x, m, n)
    if x == 1: return True
    while cnt:
        cnt -= 1
        if x == 1: return False
        if x == n - 1: return True
        x *= x
        x %= n
    return x == 1

result = miller_rabin(2, 11)
print(result)

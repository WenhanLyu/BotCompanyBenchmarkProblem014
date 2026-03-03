# Simplified pollard_rho test
def gcd(x, y):
    if y == 0: return x
    return gcd(y, x % y)

def F(n, p, c):
    return (n * n + c) % p

def iabs(x):
    if x >= 0: return x
    else: return -x

seed = 19260817

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

def is_prime(x):
    if x == 2 or x == 3 or x == 5 or x == 7: return True
    if x == 1 or x % 2 == 0: return False
    return True  # Simplified for testing

def pollard_rho(n):
    if n == 1: return
    if is_prime(n):
        print(n)
        return
    # Just test one iteration
    c = random(n - 1) + 1
    p = random(n - 1) + 1
    q = F(p, n, c)
    g = gcd(iabs(p - q), n)
    print(g)

pollard_rho(15)

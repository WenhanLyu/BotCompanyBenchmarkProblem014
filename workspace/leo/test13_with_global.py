# test13.in with global keyword added
seed = 19260817

def rand():
    global seed
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    return rand() % n

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def is_prime(n):
    if n <= 1:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def iabs(x):
    return x if x >= 0 else -x

def F(x, n, c):
    return (x * x + c) % n

def pollard_rho(n):
    if n == 1:
        return
    if is_prime(n):
        print(n)
        return
    while True:
        c, p = random(n - 1) + 1, random(n - 1) + 1
        q = F(p, n, c)
        while p != q:
            g = gcd(iabs(p - q), n)
            if g != 1 and g != n:
                pollard_rho(g)
                pollard_rho(n // g)
                return
            p, q = F(p, n, c), F(F(q, n, c), n, c)

pollard_rho(998244352)

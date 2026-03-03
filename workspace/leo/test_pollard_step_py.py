# Test a single step of Pollard's Rho
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

def F(x, n, c):
    return (x * x + c) % n

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def iabs(x):
    return x if x >= 0 else -x

# Simulate first iteration for n=998244352
n = 998244352
c, p = random(n - 1) + 1, random(n - 1) + 1
print("c =", c)
print("p =", p)

q = F(p, n, c)
print("q =", q)

g = gcd(iabs(p - q), n)
print("g =", g)

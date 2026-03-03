seed = 19260817

def rand() :
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n) : return rand() % n

def gcd(x , y) :
    if y == 0 : return x
    return gcd(y , x % y)

def iabs(x) :
    if x >= 0 : return x
    else : return -x

def F(n , p , c) : return (n * n + c) % p

def is_prime(x) :
    if x == 2 or x == 3 or x == 5 or x == 7 : return True
    if x % 2 == 0 : return False
    return True

def pollard_rho(n) :
    if n == 1 : return
    if is_prime(n) :
        print(n)
        return
    while True :
        c , p = random(n - 1) + 1 , random(n - 1) + 1
        q = F(p , n , c)
        while p != q :
            g = gcd(iabs(p - q) , n)
            if g != 1 and g != n :
                pollard_rho(g)
                pollard_rho(n // g)
                return
            p , q = F(p , n , c) , F(F(q , n , c) , n , c)

pollard_rho(12)

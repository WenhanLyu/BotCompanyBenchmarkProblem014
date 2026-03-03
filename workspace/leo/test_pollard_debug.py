seed = 19260817
call_count = 0

def rand():
    seed += seed * 131072
    seed += seed // 32
    seed += seed * 4096
    seed %= 4294967296
    return seed

def random(n):
    call_count += 1
    r = rand() % n
    print("random call", call_count, "n=", n, "result=", r)
    return r

def pollard_rho(n):
    print("pollard_rho called with n=", n)
    if n == 1:
        return
    if n == 2:
        print(n)
        return
    if n == 7:
        print(n)
        return
    if n == 17:
        print(n)
        return
    print("ERROR: Should not factor", n)

# Test factoring 998244352
pollard_rho(998244352)
print()
pollard_rho(998244352 // 7)

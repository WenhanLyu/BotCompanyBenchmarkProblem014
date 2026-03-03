def gcd(x , y) :
    if y == 0 : return x
    return gcd(y , x % y)

print(gcd(100, 50))
print(gcd(998244352, 2))
print(gcd(998244352, 7))
print(gcd(998244352, 17))

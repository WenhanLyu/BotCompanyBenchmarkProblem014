# Test evaluation order without modifying global
def f1():
    print("f1")
    return 1

def f2():
    print("f2")
    return 2

a, b = f1(), f2()
print(a)
print(b)

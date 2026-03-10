# Comprehensive Test Suite for Default Parameters
# Test Coverage: basic defaults, multiple defaults, all optional, expression defaults,
# different types, edge cases, scope tests, nested calls

print("=== Test 1: Basic default parameter ===")
def test1(a, b=5):
    print(a, b)

test1(1)
test1(1, 2)

print("=== Test 2: Multiple default parameters ===")
def test2(a, b=5, c=10, d=20):
    print(a, b, c, d)

test2(1)
test2(1, 2)
test2(1, 2, 3)
test2(1, 2, 3, 4)

print("=== Test 3: All parameters have defaults ===")
def test3(a=1, b=2):
    print(a, b)

test3()
test3(10)
test3(10, 20)

print("=== Test 4: Expression defaults - simple arithmetic ===")
def test4(a, b=1+1):
    print(a, b)

test4(5)
test4(5, 10)

print("=== Test 5: Expression defaults - multiplication ===")
def test5(a, b=3*2):
    print(a, b)

test5(3)
test5(3, 9)

print("=== Test 6: Integer defaults ===")
def test6(x=42):
    print(x)

test6()
test6(100)

print("=== Test 7: Float defaults ===")
def test7(x=3.14):
    print(x)

test7()
test7(2.71)

print("=== Test 8: String defaults ===")
def test8(s="hello"):
    print(s)

test8()
test8("world")

print("=== Test 9: Boolean defaults ===")
def test9(flag=True):
    print(flag)

test9()
test9(False)

print("=== Test 10: List defaults ===")
def test10(lst=[1, 2, 3]):
    print(lst)

test10()
test10([4, 5, 6])

print("=== Test 11: Mixed types with defaults ===")
def test11(name="Alice", age=25, score=95.5):
    print(name, age, score)

test11()
test11("Bob")
test11("Bob", 30)
test11("Bob", 30, 88.0)

print("=== Test 12: Negative number defaults ===")
def test12(a=-5, b=-10):
    print(a, b)

test12()
test12(5)
test12(5, 10)

print("=== Test 13: Zero defaults ===")
def test13(x=0, y=0):
    print(x, y)

test13()
test13(1)
test13(1, 2)

print("=== Test 14: Empty string default ===")
def test14(s=""):
    if s == "":
        print("empty")
    else:
        print(s)

test14()
test14("text")

print("=== Test 15: Complex expression defaults ===")
def test15(a, b=10+5, c=5*2):
    print(a, b, c)

test15(10)
test15(10, 20)
test15(10, 20, 50)

print("=== Test 16: Many parameters with defaults ===")
def test16(a=1, b=2, c=3, d=4, e=5):
    print(a, b, c, d, e)

test16()
test16(10)
test16(10, 20, 30)

print("=== Test 17: Function name reuse with different defaults ===")
def func_a(x=100):
    print(x)

func_a()
func_a(200)

def func_b(x=500):
    print(x)

func_b()
func_b(600)

print("=== Test 18: Defaults in recursive function ===")
def factorial(n, acc=1):
    if n <= 1:
        print(acc)
    else:
        factorial(n-1, acc*n)

factorial(5)
factorial(4, 2)

print("=== Test 19: Mixed required and optional ===")
def test19(required1, required2, opt1=10, opt2=20, opt3=30):
    print(required1, required2, opt1, opt2, opt3)

test19(1, 2)
test19(1, 2, 3)
test19(1, 2, 3, 4, 5)

print("=== Test 20: Large number default ===")
def test20(x=1000000):
    print(x)

test20()
test20(999999)

print("=== Test 21: Single parameter all optional ===")
def test21(x=42):
    print(x)

test21()
test21(100)

print("=== Test 22: String concatenation in defaults ===")
def test22(prefix="Hello", suffix="World"):
    print(prefix, suffix)

test22()
test22("Good")
test22("Good", "Bye")

print("=== All tests completed ===")

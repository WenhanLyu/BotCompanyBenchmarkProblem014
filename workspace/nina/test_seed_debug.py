seed = 100

def test1():
    print("before:", seed)
    seed += 10
    print("after:", seed)

def test2():
    print("test2 seed:", seed)

test1()
test2()
print("final seed:", seed)

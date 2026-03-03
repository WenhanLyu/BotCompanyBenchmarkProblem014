# Early return in conditional
def find_positive(x):
    if x > 0:
        return x
    return -x

print(find_positive(5))
print(find_positive(-5))

# Multiple early returns
def classify(x):
    if x > 0:
        return "positive"
    if x < 0:
        return "negative"
    return "zero"

print(classify(10))
print(classify(-10))
print(classify(0))

# Early return in loop
def find_first_even(start, end):
    i = start
    while i <= end:
        if i % 2 == 0:
            return i
        i += 1
    return -1

print(find_first_even(1, 10))
print(find_first_even(11, 20))
print(find_first_even(1, 1))

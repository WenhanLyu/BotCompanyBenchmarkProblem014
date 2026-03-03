# Test without global keyword
counter = 0

def next_val():
    counter += 1
    return counter

a, b = next_val(), next_val()
print(a)
print(b)

# Simple test to demonstrate the global variable bug

counter = 0

def increment():
    counter += 1
    return counter

print(increment())
print(increment())
print(increment())
print("Final counter:", counter)

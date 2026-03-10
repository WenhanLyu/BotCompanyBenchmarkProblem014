def should_skip(n):
    return n % 2 == 0

def should_stop(n):
    return n > 7

i = 0
while i < 15:
    i = i + 1
    if should_skip(i):
        continue
    if should_stop(i):
        break
    print(i)
print("Done")

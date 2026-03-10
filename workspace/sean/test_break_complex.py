i = 0
while i < 20:
    if i > 5 and i < 10:
        i = i + 1
        continue
    if i >= 10:
        break
    print(i)
    i = i + 1
print("Final i:", i)

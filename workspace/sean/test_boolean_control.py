i = 0
while i < 20:
    i = i + 1
    if i % 2 == 0 and i < 10:
        continue
    if i % 5 == 0 or i > 15:
        break
    print(i)
print("Done at:", i)

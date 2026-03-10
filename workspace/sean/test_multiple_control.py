i = 0
while i < 15:
    i = i + 1
    if i < 5:
        continue
    if i == 5:
        print("At 5")
    if i > 10:
        break
    print("Value:", i)
print("Final:", i)

x = 100
y = 200
i = 0
while i < 5:
    i = i + 1
    if i == 2:
        continue
    if i == 4:
        break
    print("Loop:", i)
print("x:", x)
print("y:", y)
print("i:", i)

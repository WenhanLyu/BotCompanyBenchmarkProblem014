i = 0
while i < 20:
    if i == 3:
        print("Three")
    elif i == 7:
        print("Seven")
        break
    elif i == 5:
        print("Five")
    else:
        print(i)
    i = i + 1
print("Stopped at:", i)

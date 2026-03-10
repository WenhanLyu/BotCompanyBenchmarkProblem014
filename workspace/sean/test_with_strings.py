i = 0
while i < 10:
    i = i + 1
    s = str(i)
    if s == "3":
        continue
    if s == "7":
        break
    print("Number:", s)
print("Finished")

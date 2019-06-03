myList = []
myList.append("hello")
myList.append("there")
myList.append("john")

for m in myList:
    print(m)
    
print("Printing out 2nd item in list:", myList[2])

myList[2] = "Joe"

print("Printing out 2nd item in list:", myList[2])
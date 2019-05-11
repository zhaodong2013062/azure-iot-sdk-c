x = 2
y = 5
print(x==2)
print(x==3)
print(x <= 3)

if x == 2 or y == 134324:
    print("Or condition matched")

if x == 2 and y > 4:
    print ("Condition matched")
    
if x == 5 and y > 4:
    print ("Condition NOT matched")
    
list = ["l1", "l2"]

if "l1" not in list:
    print("Surprise")
else:
    print("In list as expected")
    print("Another line because we use indentation to mark this out")
    print("Interesting...")
    
print("!!hi!!!")    
list2 = ["l1", "l2"]
print("l1==l2 <%s>" % (list==list2))
print("l1 is l2 <%s>" % (list is list2))
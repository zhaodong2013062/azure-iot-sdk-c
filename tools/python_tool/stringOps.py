str = "Hello!"

print("string <%s> has length <%d>" % (str, len(str)))
print("! comes in index <%d>" % str.index("!"))
print("Number of 'l' in string <%d>" % str.count("l"))
print("Upper <%s>" % str.upper())
split = str.split("l")
print("Split at 'l' is <%s>, 2nd is <%s>" % (split[0], split[2]))
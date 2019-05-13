# Comment here...

class MyClass:
    variable = "private"
    
    def printVar(self):
        print(self.variable)
        
    def setVar(self,newVar):
        self.variable = newVar
        
myc = MyClass()

myc.printVar()
myc.setVar("reset")
myc.printVar()
class Node(object):
    def __init__(self, left, right):
        self.left = left
        self.right = right

class PlusNode(Node):
    def eval(self):
        return self.right.eval() + self.left.eval()
    def visit(self):
        return '+'

class MinusNode(Node):
    def eval(self):
        return self.right.eval() - self.left.eval()
    def visit(self):
        return '-'

class TimesNode(Node):
    def eval(self):
        return self.right.eval() * self.left.eval()
    def visit(self):
        return '*'
class DivideNode(Node):
    def eval(self):
        return self.right.eval() / self.left.eval()
    def visit(self):
        return '/'
    
class ExpNode(Node):
    def eval(self):
        return self.right.eval() ** self.left.eval()
    def visit(self):
        return '^'

class NumNode(Node):
    def __init__(self, x):
        super(NumNode,self).__init__(None, None)
        self.num = x
    def eval(self):
        return self.num
    def visit(self):
        return str(self.num)

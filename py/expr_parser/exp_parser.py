from queue import Queue
from node import *

operators = ['+','-','*','/','^']
l_parens = ['(', '[', '{']
r_parens = [')', ']', '}']

def tokenize_exp(strexp):
    raw_exp = list(strexp)
    raw_exp = filter(lambda a: a!=' ',raw_exp)

    q = Queue()
    for token in raw_exp: q.enqueue(token)

    exp=[]
    strnum = ""
    ops = operators + l_parens + r_parens
    
    while q.isEmpty()==False:
        token = q.dequeue()
        if ord('9') >= ord(token) >= ord('0'):
            strnum += token
        elif token in ops:
            if strnum!="": exp.append(strnum)
            exp.append(token)
            strnum = ""
        else: raise RuntimeError("illegal token/operator in exp")

    if strnum: exp.append(strnum)
    return exp

def verify_parens(tokens):
    stack_lp=[]
    for t in tokens:
        if t in l_parens:
            stack_lp.append(t)
        elif t in r_parens:
            if stack_lp==[]:
                raise RuntimeError("extra right paranthesis!!")
            else:
                p = stack_lp.pop()
                if (p=='(' and t!=')') or (p=='[' and t!=']') or (p=='{' and t!='}'):
                    raise RuntimeError("paranthesis mismatch!!")
                    
    if stack_lp!=[]: raise RuntimeError("unbalanced parantheses!!")
    print "paranthesis verification passed"
    return True
                                
def op_prec(operator):
    prec0=['+','-']
    prec1=['*','/']
    prec2=['^']
    if operator in prec0: return 0
    elif operator in prec1: return 1
    elif operator in prec2: return 2
    else: raise RuntimeError("Invalid operator!!")
        
def to_postfix(tokens):
    pf_exp = []
    op_stack = [[]]
    q = Queue()
    for t in tokens: q.enqueue(t)

    while q.isEmpty()==False:
        token = q.dequeue()
        stack = op_stack[-1]
        if token in operators:
            while stack!=[] and op_prec(stack[-1]) >= op_prec(token):
                pf_exp.append(stack.pop())
            stack.append(token)
        elif token in l_parens:
            op_stack.append([])
        elif token in r_parens:
            while stack!=[]: pf_exp.append(stack.pop())
            del op_stack[-1]
        else:
            pf_exp.append(token)
    stack = op_stack[0]
    while stack!=[]: pf_exp.append(stack.pop())
    return pf_exp

def build_AST(pf_exp):
    if pf_exp==[]: return
    t = pf_exp.pop()
    if   t=='+': return PlusNode(build_AST(pf_exp), build_AST(pf_exp))
    elif t=='-': return MinusNode(build_AST(pf_exp), build_AST(pf_exp))
    elif t=='*': return TimesNode(build_AST(pf_exp), build_AST(pf_exp))
    elif t=='/': return DivideNode(build_AST(pf_exp), build_AST(pf_exp))
    elif t=='^': return ExpNode(build_AST(pf_exp), build_AST(pf_exp))
    else: return NumNode(int(t))

def inorder(root):
    def recurse(root):
        if root.left  != None: recurse(root.left)
        traversal.append(root.visit())
        if root.right != None: recurse(root.right)
    traversal = []
    recurse(root)
    return traversal

def verify_grammar(ast_root):
    trav = inorder(ast_root)
    nums = trav[::2]
    numeric  = [t.isdigit() for t in nums]
    if not all(numeric) or trav[-1].isdigit()==False:
        raise RuntimeError("grammar verification failed!!")
    print "grammar verification passed"
    return True

def eval_exp(expression):
    tokens = tokenize_exp(expression)
    print "tokens read:\n", tokens
    verify_parens(tokens)
    pfix = to_postfix(tokens)
    print "postfix notation:\n", pfix
    ast_root = build_AST(pfix)
    print "built AST Tree.."
    traversal = inorder(ast_root)
    print "in-order tree traversal:\n", traversal
    verify_grammar(ast_root)
    res = ast_root.eval();
    print "result computed =", res
    return True
        
def main():
    expression = raw_input("enter an algebraic expression (integers): ")
    eval_exp(expression)

if __name__ == "__main__": main()

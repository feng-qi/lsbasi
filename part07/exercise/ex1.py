from spi_class import Lexer, Parser, NodeVisitor

class ReversePolishNotation(NodeVisitor):
    def __init__(self, parser):
        self.parser = parser

    def visit_BinOp(self, node):
        return f'{self.visit(node.left)} {self.visit(node.right)} {node.op.value}'
        # if node.op.type == PLUS:
        #     return ' '.join([self.visit(node.left), self.visit(node.right), '+'])
        # elif node.op.type == MINUS:
        #     return ' '.join([self.visit(node.left), self.visit(node.right), '-'])
        # elif node.op.type == MUL:
        #     return ' '.join([self.visit(node.left), self.visit(node.right), '*'])
        # elif node.op.type == DIV:
        #     return ' '.join([self.visit(node.left), self.visit(node.right), '/'])

    def visit_Num(self, node):
        return node.value

    def getRPN(self):
        return self.visit(self.parser.parse())

def main():
    while True:
        try:
            text = input('RPN> ')
        except EOFError:
            print()
            break

        if len(text.strip()):
            parser = Parser(Lexer(text))
            rpn = ReversePolishNotation(parser)
            result = rpn.getRPN()
            print(result)

if __name__ == '__main__':
    main()

from spi_class import Lexer, Parser, NodeVisitor

class LISPNotation(NodeVisitor):
    def __init__(self, parser):
        self.parser = parser

    def visit_BinOp(self, node):
        return f'({node.op.value} {self.visit(node.left)} {self.visit(node.right)})'

    def visit_Num(self, node):
        return node.value

    def getLISP(self):
        return self.visit(self.parser.parse())

def main():
    while True:
        try:
            text = input('LISP> ')
        except EOFError:
            print()
            break

        if len(text.strip()):
            parser = Parser(Lexer(text))
            lisp = LISPNotation(parser)
            result = lisp.getLISP()
            print(result)

if __name__ == '__main__':
    main()

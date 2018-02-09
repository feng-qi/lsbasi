(INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, EOF,
 BEGIN, END, DOT, SEMI, ID, ASSIGN) = (
     'INTEGER', 'PLUS', 'MINUS', 'MUL', 'DIV', '(', ')', 'EOF',
     'BEGIN', 'END', 'DOT', 'SEMI', 'ID', 'ASSIGN'
)

class Token():
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return f'Token({self.type}, {repr(self.value)})'

    def __repr__(self):
        return self.__str__()

RESERVED_KEYWORDS = {
    'BEGIN': Token(BEGIN, 'BEGIN'),
    'END': Token(END, 'END'),
    'DIV': Token(DIV, 'DIV'),
}

class Lexer():
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos]

    def error(self):
        raise Exception('Invalid character')

    def advance(self):
        self.pos += 1
        if self.pos < len(self.text):
            self.current_char = self.text[self.pos]
        else:
            self.current_char = None

    def peek(self):
        peek_pos = self.pos + 1
        if peek_pos >= len(self.text):
            return None
        else:
            return self.text[peek_pos]

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def _id(self):
        value = ''
        if self.current_char == '_':
            value = '_'
            self.advance()

        while self.current_char is not None and self.current_char.isalnum():
            value += self.current_char
            self.advance()
        return RESERVED_KEYWORDS.get(value.upper(), Token(ID, value))

    def integer(self):
        value = ''
        while self.current_char is not None and self.current_char.isdigit():
            value += self.current_char
            self.advance()
        return int(value)

    def get_next_token(self):
        while self.current_char is not None:
            if self.current_char.isspace():
                self.skip_whitespace()
                continue
            elif self.current_char.isalpha() or self.current_char == '_':
                return self._id()
            elif self.current_char.isdigit():
                return Token(INTEGER, self.integer())
            elif self.current_char == ':' and self.peek() == '=':
                self.advance()
                self.advance()
                return Token(ASSIGN, ':=')
            elif self.current_char == '.':
                self.advance()
                return Token(DOT, '.')
            elif self.current_char == ';':
                self.advance()
                return Token(SEMI, ';')
            elif self.current_char == '+':
                self.advance()
                return Token(PLUS, '+')
            elif self.current_char == '-':
                self.advance()
                return Token(MINUS, '-')
            elif self.current_char == '*':
                self.advance()
                return Token(MUL, '*')
            # elif self.current_char == '/':
            #     self.advance()
            #     return Token(DIV, '/')
            elif self.current_char == '(':
                self.advance()
                return Token(LPAREN, '(')
            elif self.current_char == ')':
                self.advance()
                return Token(RPAREN, ')')
            else:
                self.error()
        return Token(EOF, None)

###############################################################################
#                                                                             #
#  PARSER                                                                     #
#                                                                             #
###############################################################################

class AST():
    pass

class BinOp(AST):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right

class UnaryOp(AST):
    def __init__(self, op, expr):
        self.token = self.op = op
        self.expr = expr

class Num(AST):
    def __init__(self, token):
        self.token = token
        self.value = token.value

class Compound(AST):
    def __init__(self):
        self.children = []

class Assign(AST):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right

class Var(AST):
    def __init__(self, token):
        self.token = token
        self.value = token.value

class NoOp(AST):
    pass

class Parser():
    def __init__(self, lexer):
        self.lexer = lexer
        # set current token to the first token taken from the input
        self.current_token = lexer.get_next_token()

    def error(self):
        raise Exception('Invalid syntax')

    def eat(self, token_type):
        # compare the current token type with the passed token
        # type and if they match then "eat" the current token
        # and assign the next token to the self.current_token,
        # otherwise raise an exception.
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    def variable(self):
        node = Var(self.current_token)
        self.eat(ID)
        return node

    def factor(self):
        """factor : (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN"""
        token = self.current_token
        if token.type == PLUS:
            self.eat(PLUS)
            return UnaryOp(op=token, expr=self.factor())
        elif token.type == MINUS:
            self.eat(MINUS)
            return UnaryOp(op=token, expr=self.factor())
        elif token.type == INTEGER:
            self.eat(INTEGER)
            return Num(token)
        elif token.type == LPAREN:
            self.eat(LPAREN)
            node = self.expr()
            self.eat(RPAREN)
            return node
        else:
            return self.variable()

    def term(self):
        """term : factor ((MUL | DIV) factor)*"""
        node = self.factor()

        while self.current_token.type in (MUL, DIV):
            token = self.current_token
            if token.type == MUL:
                self.eat(MUL)
            elif token.type == DIV:
                self.eat(DIV)

            node = BinOp(left=node, op=token, right=self.factor())

        return node

    def expr(self):
        """
        expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : INTEGER | LPAREN expr RPAREN
        """
        node = self.term()

        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            if token.type == PLUS:
                self.eat(PLUS)
            elif token.type == MINUS:
                self.eat(MINUS)

            node = BinOp(left=node, op=token, right=self.term())

        return node

    def empty(self):
        return NoOp()

    def assignment_statement(self):
        left = self.variable()
        op = self.current_token
        self.eat(ASSIGN)
        right = self.expr()
        return Assign(left, op, right)

    def statement(self):
        if self.current_token.type == BEGIN:
            return self.compound_statement()
        elif self.current_token.type == ID:
            return self.assignment_statement()
        else:
            return self.empty()

    def statement_list(self):
        node = self.statement()
        nodes = [node]
        while self.current_token.type == SEMI:
            self.eat(SEMI)
            nodes.append(self.statement())

        if self.current_token.type == ID:
            self.error()
        return nodes

    def compound_statement(self):
        self.eat(BEGIN)
        nodes = self.statement_list()
        self.eat(END)

        root = Compound()
        for node in nodes:
            root.children.append(node)
        return root

    def program(self):
        node = self.compound_statement()
        self.eat(DOT)
        return node

    def parse(self):
        node = self.program()
        if self.current_token.type != EOF:
            self.error()
        return node

###############################################################################
#                                                                             #
#  INTERPRETER                                                                #
#                                                                             #
###############################################################################

class NodeVisitor():
    def visit(self, node):
        method_name = 'visit_' + type(node).__name__
        visitor = getattr(self, method_name, self.generic_visit)
        return visitor(node)

    def generic_visit(self, node):
        raise Exception('No visitor_{} method'.format(type(node).__name__))

class Interpreter(NodeVisitor):
    GLOBAL_SCOPE = {}
    def __init__(self, parser):
        self.parser = parser

    def visit_BinOp(self, node):
        if node.op.type == PLUS:
            return self.visit(node.left) + self.visit(node.right)
        elif node.op.type == MINUS:
            return self.visit(node.left) - self.visit(node.right)
        elif node.op.type == MUL:
            return self.visit(node.left) * self.visit(node.right)
        elif node.op.type == DIV:
            return self.visit(node.left) / self.visit(node.right)

    def visit_UnaryOp(self, node):
        op = node.op
        if op.type == PLUS:
            return +self.visit(node.expr)
        elif op.type == MINUS:
            return -self.visit(node.expr)

    def visit_Num(self, node):
        return node.value

    def visit_NoOp(self, node):
        pass

    def visit_Var(self, node):
        var_name = node.value
        value = self.GLOBAL_SCOPE.get(var_name.lower())
        if value is None:
            raise NameError(repr(value))
        else:
            return value

    def visit_Assign(self, node):
        var_name = node.left.value
        self.GLOBAL_SCOPE[var_name.lower()] = self.visit(node.right)

    def visit_Compound(self, node):
        for child in node.children:
            self.visit(child)

    def interpret(self):
        tree = self.parser.parse()
        if tree is None:
            return ''
        return self.visit(tree)

def main():
    import sys
    text = open(sys.argv[1], 'r').read()

    interpreter = Interpreter(Parser(Lexer(text)))
    interpreter.interpret()
    print(interpreter.GLOBAL_SCOPE)

if __name__ == '__main__':
    main()

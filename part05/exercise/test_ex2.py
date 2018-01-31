import unittest
from ex2 import *

class TestLexer(unittest.TestCase):

    def test_lexer(self):
        lexer = Lexer('1 + 23 - 456 * 78 / 9')
        self.assertEqual( str(lexer.get_next_token()), "Token(INTEGER, 1)"  )
        self.assertEqual( str(lexer.get_next_token()), "Token(PLUS, '+')"   )
        self.assertEqual( str(lexer.get_next_token()), "Token(INTEGER, 23)" )
        self.assertEqual( str(lexer.get_next_token()), "Token(MINUS, '-')"  )
        self.assertEqual( str(lexer.get_next_token()), "Token(INTEGER, 456)")
        self.assertEqual( str(lexer.get_next_token()), "Token(MUL, '*')"    )
        self.assertEqual( str(lexer.get_next_token()), "Token(INTEGER, 78)" )
        self.assertEqual( str(lexer.get_next_token()), "Token(DIV, '/')"    )
        self.assertEqual( str(lexer.get_next_token()), "Token(INTEGER, 9)"  )

class TestInterpreter(unittest.TestCase):

    def test_interpreter(self):
        interpreter = Interpreter(Lexer('1'))
        self.assertEqual( interpreter.parse(), 1)
        interpreter = Interpreter(Lexer('23'))
        self.assertEqual( interpreter.parse(), 23)
        interpreter = Interpreter(Lexer('456'))
        self.assertEqual( interpreter.parse(), 456)
        interpreter = Interpreter(Lexer('1 + 23 - 456 * 78 / 9'))
        self.assertEqual( interpreter.parse(), -3928)
        # '−' is not '-', although they look the same, T.T
        interpreter = Interpreter(Lexer('112 + 23- 24'))
        self.assertEqual( interpreter.parse(), 111)

        interpreter = Interpreter(Lexer('112 + (23- 24)'))
        self.assertEqual( interpreter.parse(), 111)

        interpreter = Interpreter(Lexer('7 + 3 * (10 / (12 / (3 + 1) - 1))'))
        self.assertEqual( interpreter.parse(), 22)
        interpreter = Interpreter(Lexer('7 + 3 * (10 / (12 / (3) + 1)) + 123'))
        self.assertEqual( interpreter.parse(), 136)

if __name__ == '__main__':
    unittest.main()

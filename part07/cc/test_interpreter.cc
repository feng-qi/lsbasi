#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Interpreter
#include <boost/test/unit_test.hpp>
#include "Interpreter.h"
#include <string>

BOOST_AUTO_TEST_CASE(test_interpreter)
{
    // AST* root   = new AST(Token::PLUS, 0, nullptr, nullptr);
    // root->left  = new AST(Token::NUM,  1, nullptr, nullptr);
    // root->right = new AST(Token::NUM,  1, nullptr, nullptr);
    Interpreter interpreter{Parser{Lexer{"1+1"}}};
    BOOST_CHECK(interpreter.interpret() == 2);

    interpreter.reload(Parser{Lexer{"3 * (1+1)"}});
    BOOST_CHECK(interpreter.interpret() == 6);

    interpreter.reload(Parser{Lexer{"7 + 3 * (10 / (12 / (3 + 1) - 1))"}});
    BOOST_CHECK(interpreter.interpret() == 22);

    interpreter.reload(Parser{Lexer{"7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)"}});
    BOOST_CHECK(interpreter.interpret() == 10);

    interpreter.reload(Parser{Lexer{"7 + (((3 + 2)))"}});
    BOOST_CHECK(interpreter.interpret() == 12);
}

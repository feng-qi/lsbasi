#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Interpreter
#include <boost/test/unit_test.hpp>
#include "Interpreter.h"
#include <string>

BOOST_AUTO_TEST_CASE(test_Lexer)
{
    Lexer lexer("1 + 23 - 456 * 78 / 9");
    BOOST_CHECK( Token::DOUBLE   == lexer.get_next_token() );
    BOOST_CHECK( 1            == lexer.dvalue           );
    BOOST_CHECK( Token::PLUS  == lexer.get_next_token() );
    BOOST_CHECK( Token::DOUBLE   == lexer.get_next_token() );
    BOOST_CHECK( 23           == lexer.dvalue           );
    BOOST_CHECK( Token::MINUS == lexer.get_next_token() );
    BOOST_CHECK( Token::DOUBLE   == lexer.get_next_token() );
    BOOST_CHECK( 456          == lexer.dvalue           );
    BOOST_CHECK( Token::MUL   == lexer.get_next_token() );
    BOOST_CHECK( Token::DOUBLE   == lexer.get_next_token() );
    BOOST_CHECK( 78           == lexer.dvalue           );
    BOOST_CHECK( Token::DIV   == lexer.get_next_token() );
    BOOST_CHECK( Token::DOUBLE   == lexer.get_next_token() );
    BOOST_CHECK( 9            == lexer.dvalue           );
    BOOST_CHECK( Token::EOI   == lexer.get_next_token() );
    BOOST_CHECK( Token::EOI   == lexer.get_next_token() );
}

BOOST_AUTO_TEST_CASE(test_Interpreter) {
    Interpreter interpreter{ Lexer("1")};               BOOST_CHECK_EQUAL(interpreter.parse(), 1);
    interpreter.reload(Lexer("23"));                    BOOST_CHECK_EQUAL(interpreter.parse(), 23);
    interpreter.reload(Lexer("456"));                   BOOST_CHECK_EQUAL(interpreter.parse(), 456);
    interpreter.reload(Lexer("1 + 23 - 456 * 78 / 9")); BOOST_CHECK_EQUAL(interpreter.parse(), -3928);
    interpreter.reload(Lexer("112 + 23- 24"));          BOOST_CHECK_EQUAL(interpreter.parse(), 111);

    interpreter.reload(Lexer("()"));                    BOOST_CHECK_EQUAL(interpreter.parse(), 0);
    interpreter.reload(Lexer("(23)"));                  BOOST_CHECK_EQUAL(interpreter.parse(), 23);
    interpreter.reload(Lexer("(112 + 23)- 24"));        BOOST_CHECK_EQUAL(interpreter.parse(), 111);
    interpreter.reload(Lexer("112 + (23- 24)"));        BOOST_CHECK_EQUAL(interpreter.parse(), 111);
    interpreter.reload(Lexer("2 * (7 + 3)"));           BOOST_CHECK_EQUAL(interpreter.parse(), 20);
    interpreter.reload(Lexer("7 + 3 * (10 / (12 / (3 + 1) - 1))"));
    BOOST_CHECK_EQUAL(interpreter.parse(), 22);
}

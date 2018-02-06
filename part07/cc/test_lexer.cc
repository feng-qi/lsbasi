#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Interpreter
#include <boost/test/unit_test.hpp>
#include "Interpreter.h"
#include <string>

BOOST_AUTO_TEST_CASE(test_no_parenthesis)
{
    Lexer lexer("1 + 23 - 456 * 78 / 9");
    BOOST_CHECK( Token::NUM   == lexer.get_next_token() );
    BOOST_CHECK( 1            == lexer.dvalue           );
    BOOST_CHECK( Token::PLUS  == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM   == lexer.get_next_token() );
    BOOST_CHECK( 23           == lexer.dvalue           );
    BOOST_CHECK( Token::MINUS == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM   == lexer.get_next_token() );
    BOOST_CHECK( 456          == lexer.dvalue           );
    BOOST_CHECK( Token::MUL   == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM   == lexer.get_next_token() );
    BOOST_CHECK( 78           == lexer.dvalue           );
    BOOST_CHECK( Token::DIV   == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM   == lexer.get_next_token() );
    BOOST_CHECK( 9            == lexer.dvalue           );
    BOOST_CHECK( Token::EOI   == lexer.get_next_token() );
    BOOST_CHECK( Token::EOI   == lexer.get_next_token() );
}

BOOST_AUTO_TEST_CASE(test_parenthesis)
{
    Lexer lexer("7 + 3 * (10 / (12 / (3 + 1) - 1))");
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() ); BOOST_CHECK( 7  == lexer.dvalue );
    BOOST_CHECK( Token::PLUS   == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() ); BOOST_CHECK( 3  == lexer.dvalue );
    BOOST_CHECK( Token::MUL    == lexer.get_next_token() );
    BOOST_CHECK( Token::LPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() ); BOOST_CHECK( 10 == lexer.dvalue );
    BOOST_CHECK( Token::DIV    == lexer.get_next_token() );
    BOOST_CHECK( Token::LPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() ); BOOST_CHECK( 12 == lexer.dvalue );
    BOOST_CHECK( Token::DIV    == lexer.get_next_token() );
    BOOST_CHECK( Token::LPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() );
    BOOST_CHECK( 3             == lexer.dvalue           );
    BOOST_CHECK( Token::PLUS   == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() ); BOOST_CHECK( 1  == lexer.dvalue );
    BOOST_CHECK( Token::RPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::MINUS  == lexer.get_next_token() );
    BOOST_CHECK( Token::NUM    == lexer.get_next_token() );
    BOOST_CHECK( 1             == lexer.dvalue           );
    BOOST_CHECK( Token::RPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::RPAREN == lexer.get_next_token() );
    BOOST_CHECK( Token::EOI    == lexer.get_next_token() );
    BOOST_CHECK( Token::EOI    == lexer.get_next_token() );
}

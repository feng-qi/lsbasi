#include "parser.h"
#include <string>
#include <stdexcept>
#include <cctype>

// Lexer ---------------------------------------------------------
void Lexer::error() {
    throw std::runtime_error("Lexer error");
}

void Lexer::skip_whitespace() {
    while (pos < text.size() && isspace(text[pos]))
        ++pos;
}

int Lexer::integer() {
    int value = 0;
    while (isdigit(text[pos])) {
        value = value * 10 + text[pos] - '0';
        ++pos;
    }
    return value;
}

Token Lexer::get_next_token() {
    while (pos < text.size()) {
        if (isspace(text[pos])) {
            skip_whitespace();
            continue;
        } else if (isdigit(text[pos])) {
            ivalue = integer();
            return Token::INT;
        } else if (text[pos] == '*') {
            ++pos;
            return Token::MUL;
        } else if (text[pos] == '/') {
            ++pos;
            return Token::DIV;
        } else
            error();
    }
    return Token::EOI;
}

// Parser ---------------------------------------------------------
void Parser::error() {
    throw std::runtime_error("Parser error");
}

void Parser::eat(Token token) {
    if (cur_token == token) {
        cur_token = lexer.get_next_token();
    } else {
        error();
    }
}

void Parser::factor() {
    eat(Token::INT);
}

void Parser::expr() {
    factor();

    while (cur_token == Token::MUL || cur_token == Token::DIV) {
        if (cur_token == Token::MUL) {
            eat(Token::MUL);
        } else if (cur_token == Token::DIV) {
            eat(Token::DIV);
        }
        factor();
    }
}

void Parser::parse() {
    expr();
}

#include "Interpreter.h"
#include <stdexcept>
#include <cctype>

// Lexer ---------------------------------------------
void Lexer::error() {
    throw std::runtime_error("Lexer error");
}

void Lexer::skip_whitespace() {
    while (pos < text.size() && isspace(text[pos])) {
        ++pos;
    }
}

int Lexer::integer() {
    int value = 0;
    while (pos < text.size() && isdigit(text[pos])) {
        value += value * 10 + text[pos] - '0';
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

// Interpreter ---------------------------------------------
inline void Interpreter::error() {
    throw std::runtime_error("Interpreter error");
}

void Interpreter::eat(Token token) {
    if (cur_token == token)
        cur_token = lexer.get_next_token();
    else
        error();
}

int Interpreter::factor() {
    int value = lexer.ivalue;
    eat(Token::INT);
    return value;
}

int Interpreter::expr() {
    int value = factor();

    while (cur_token == Token::MUL || cur_token == Token::DIV) {
        if (cur_token == Token::MUL) {
            eat(Token::MUL);
            value *= factor();
        } else if (cur_token == Token::DIV) {
            eat(Token::DIV);
            value /= factor();
        }
    }
    return value;
}

int Interpreter::parse() {
    return expr();
}

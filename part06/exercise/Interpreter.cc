#include "Interpreter.h"
#include <cctype>

// Lexer --------------------------------------------
Token Lexer::get_next_token() {
    while (pos < text.size()) {
        if (isspace(text[pos])) {
            skip_whitespace();
            continue;
        } else if (isdigit(text[pos])) {
            dvalue = get_double();
            return Token::DOUBLE;
        } else if (text[pos] == '+') {
            ++pos;
            return Token::PLUS;
        } else if (text[pos] == '-') {
            ++pos;
            return Token::MINUS;
        } else if (text[pos] == '*') {
            ++pos;
            return Token::MUL;
        } else if (text[pos] == '/') {
            ++pos;
            return Token::DIV;
        } else if (text[pos] == '(') {
            ++pos;
            return Token::LPAREN;
        } else if (text[pos] == ')') {
            ++pos;
            return Token::RPAREN;
        } else
            error();
    }
    return Token::EOI;
}

void Lexer::skip_whitespace() {
    while (pos < text.size() && isspace(text[pos]))
        ++pos;
}

double Lexer::get_double() {
    double value = 0;
    while (pos < text.size() && isdigit(text[pos])) {
        value = value * 10 + text[pos] - '0';
        ++pos;
    }
    return value;
}

// Interpreter --------------------------------------------
void Interpreter::eat(Token token) {
    if (cur_token == token)
        cur_token = lexer.get_next_token();
    else
        error();
}

double Interpreter::factor() {
    double value = 0;
    if (cur_token == Token::DOUBLE) {
        value = lexer.dvalue;
        eat(Token::DOUBLE);
    } else if (cur_token == Token::LPAREN) {
        eat(Token::LPAREN);
        value = expr();
        eat(Token::RPAREN);
    }
    return value;
}

double Interpreter::term() {
    double value = factor();

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

double Interpreter::expr() {
    double value = term();

    while (cur_token == Token::PLUS || cur_token == Token::MINUS) {
        if (cur_token == Token::PLUS) {
            eat(Token::PLUS);
            value += term();
        } else if (cur_token == Token::MINUS) {
            eat(Token::MINUS);
            value -= term();
        }
    }
    return value;
}

void Interpreter::reload(Lexer _lexer) {
    lexer = _lexer;
    cur_token = lexer.get_next_token();
}

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>

enum class Token {
    INT, MUL, DIV, EOI
};

class Lexer {
public:
    Lexer(const std::string& _text): text(_text) {}
    Token get_next_token();

private:
    void error();
    void skip_whitespace();
    int  integer();

public:
    int ivalue;
private:
    const std::string text;
    int pos = 0;
};

class Parser {
public:
    Parser(Lexer _lexer): lexer(_lexer) {
        cur_token = lexer.get_next_token();
    }
    void parse();

private:
    void error();
    void factor();
    void eat(Token token);
    void expr();

public:
    Token cur_token;

private:
    Lexer lexer;
    // Token cur_token;
};

#endif  // PARSER_H

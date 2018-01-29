#ifndef INTERPRETER_H
#define INTERPRETER_H

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
    int pos = 0;
    std::string text;
};

class Interpreter {
public:
    Interpreter(Lexer _lexer): lexer(_lexer) {
        cur_token = lexer.get_next_token();
    }
    int parse();

private:
    void eat(Token token);
    void error();
    int  factor();
    int  expr();

private:
    Token cur_token;
    Lexer lexer;
};

#endif  // INTERPRETER_H

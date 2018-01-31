#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <stdexcept>

enum class Token {
    INT, PLUS, MINUS, MUL, DIV, EOI
};

class Lexer {
public:
    Lexer(const std::string& _text): text(_text) {}
    Token get_next_token();
    void  error() { throw std::runtime_error("Lexer error"); };
    void  skip_whitespace();
    int   integer();

    int ivalue = 0;

private:
    std::string text;
    int pos = 0;
};

class Interpreter {
public:
    Interpreter(Lexer l): lexer(l) { cur_token = lexer.get_next_token(); }
    void reload(Lexer l) { lexer = l; cur_token = lexer.get_next_token(); };
    void error() { throw std::runtime_error("Lexer error"); };
    void eat(Token token);
    int  parse();
    int  expr();
    int  term();
    int  factor();

private:
    Lexer lexer;
    Token cur_token;
};

#endif /* INTERPRETER_H */

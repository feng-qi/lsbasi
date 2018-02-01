#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <stdexcept>

enum class Token {
    DOUBLE, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, EOI
};

class Lexer {
public:
    Lexer(const std::string& _text): text(_text) {}
    Token get_next_token();
    void error() { throw std::runtime_error("Lexer error"); }
    void skip_whitespace();
    double get_double();

    double dvalue;
private:
    std::string text;
    int pos = 0;
};

class Interpreter {
// expr   : term   ((PLUS | MINUS) term)*
// term   : factor ((MUL | DIV) factor)*
// factor : INTEGER | LPAREN expr RPAREN
public:
    Interpreter(Lexer _lexer): lexer(_lexer) { cur_token = lexer.get_next_token(); }
    double parse() { return expr(); }
    double expr();
    double term();
    double factor();
    void error() { throw std::runtime_error("Interpreter error"); }
    void eat(Token token);

    void reload(Lexer _lexer);              // for the convenience of unit test

private:
    Lexer lexer;
    Token cur_token;
};

#endif /* INTERPRETER_H */

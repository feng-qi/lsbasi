#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <stdexcept>
#include <memory>

enum class Token {
    NUM, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, EOI
};

class Lexer {
public:
    Lexer(const std::string& _text): text(_text) {}
    Token get_next_token();

    double dvalue;

private:
    void error() { throw std::runtime_error("Invalid character"); }
    void skip_whitespace();
    double number();

    std::string text;
    std::string::size_type pos = 0;
};

struct AST {
    AST(Token t, double d, AST* l, AST* r) : token(t), dvalue(d), left(l), right(r) {}
    Token token;
    double dvalue;
    AST* left;
    AST* right;
};

class Parser {
public:
    Parser(Lexer _lexer): lexer(_lexer) { cur_token = lexer.get_next_token(); }
    AST* factor();
    AST* term();
    AST* expr();
    AST* parse();

private:
    void error() { throw std::runtime_error("Invalid syntax"); }
    void eat(Token token);

    Lexer lexer;
    Token cur_token;
};

class Interpreter {
public:
    Interpreter(Parser _parser) : parser(_parser) {}
    double interpret();

private:
    void error() { throw std::runtime_error("Invalid AST"); }
    double visit(AST* node);
    double visit_num(AST* node);
    double visit_op(AST* node);

    Parser parser;
};

#endif /* INTERPRETER_H */

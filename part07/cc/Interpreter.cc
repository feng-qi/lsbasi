#include "Interpreter.h"
#include <string>
#include <cctype>

// Lexer ----------------------------------------------------
Token Lexer::get_next_token() {
    while (pos < text.size()){
        if (isspace(text[pos])) {
            skip_whitespace();
            continue;
        } else if (isdigit(text[pos])) {
            dvalue = number();
            return Token::NUM;
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
    // while (pos < text.size()){
    //     if (isspace(text[pos])) {
    //         skip_whitespace();
    //         continue;
    //     } else if (isdigit(text[pos])) {
    //         dvalue = number();
    //         return Token::NUM;
    //     } else {
    //         Token token;
    //         switch (text[pos]) {
    //         case '+':
    //             token = Token::PLUS;   break;
    //         case '-':
    //             token = Token::MINUS;  break;
    //         case '*':
    //             token = Token::MUL;    break;
    //         case '/':
    //             token = Token::DIV;    break;
    //         case '(':
    //             token = Token::LPAREN; break;
    //         case ')':
    //             token = Token::RPAREN; break;
    //         default:
    //             error();
    //         }
    //         ++pos;
    //         return token;
    //     }
    // }
    return Token::EOI;
}

void Lexer::skip_whitespace() {
    while (pos < text.size() && isspace(text[pos]))
        ++pos;
}

double Lexer::number() {
    double value = 0;
    while (pos < text.size() && isdigit(text[pos])) {
        value = value * 10 + text[pos] - '0';
        ++pos;
    }
    return value;
}

// Parser ----------------------------------------------------
std::shared_ptr<AST> Parser::factor() {
    Token token = cur_token;
    std::shared_ptr<AST> node;
    if (token == Token::NUM) {
        node = std::make_shared<AST>(Token::NUM, lexer.dvalue, nullptr, nullptr);
        eat(Token::NUM);
    } else if (cur_token == Token::LPAREN) {
        eat(Token::LPAREN);
        node = expr();
        eat(Token::RPAREN);
    }
    return node;
}

std::shared_ptr<AST> Parser::term() {
    std::shared_ptr<AST> node = factor();

    while (cur_token == Token::MUL || cur_token == Token::DIV) {
        if (cur_token == Token::MUL) {
            eat(Token::MUL);
            // node = new AST(Token::MUL, 0, node, factor());
            node = std::make_shared<AST>(Token::MUL, 0, node, factor());
        } else if (cur_token == Token::DIV) {
            eat(Token::DIV);
            // node = new AST(Token::DIV, 0, node, factor());
            node = std::make_shared<AST>(Token::DIV, 0, node, factor());
        }
    }
    return node;
}

std::shared_ptr<AST> Parser::expr() {
    std::shared_ptr<AST> node = term();

    while (cur_token == Token::PLUS || cur_token == Token::MINUS) {
        if (cur_token == Token::PLUS) {
            eat(Token::PLUS);
            // node = new AST(Token::PLUS, 0, node, term());
            node = std::make_shared<AST>(Token::PLUS, 0, node, term());
        } else if (cur_token == Token::MINUS) {
            eat(Token::MINUS);
            // node = new AST(Token::MINUS, 0, node, term());
            node = std::make_shared<AST>(Token::MINUS, 0, node, term());
        }
    }
    return node;
}

std::shared_ptr<AST> Parser::parse() {
    return expr();
}

void Parser::eat(Token token) {
    if (cur_token == token)
        cur_token = lexer.get_next_token();
    else
        error();
}

// Interpreter ----------------------------------------------------
double Interpreter::visit(std::shared_ptr<AST> node) {
    switch (node->token) {
    case Token::NUM:
        return visit_num(node);
        break;
    case Token::MINUS:
    case Token::PLUS:
    case Token::MUL:
    case Token::DIV:
        return visit_op(node);
        break;
    default:
        error();
    }
    error();
    return 0;
}

double Interpreter::visit_num(std::shared_ptr<AST> node) {
    return node->dvalue;
}

double Interpreter::visit_op(std::shared_ptr<AST> node) {
    switch (node->token) {
    case Token::PLUS:
        return visit(node->left) + visit(node->right);
    case Token::MINUS:
        return visit(node->left) - visit(node->right);
    case Token::MUL:
        return visit(node->left) * visit(node->right);
    case Token::DIV:
        return visit(node->left) / visit(node->right);
    default:
        error();
    }
    error();
    return 0;
}

double Interpreter::interpret() {
    return visit(parser.parse());
}

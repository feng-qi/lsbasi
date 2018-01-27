#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

enum class Token {
    NUMBER,
    PLUS,
    MINUS,
    TEOF
};

class Interpreter {
public:
    Interpreter(std::string& in): text(in) {}
    int expr();

private:
    Token get_next_token();
    void  skip_whitespace();
    void  eat(Token type);
    void  error();
    int   get_number();
    int   item();

    std::string text;
    int         pos       = 0;
    int         cur_value;
    Token       cur_token;
};

void Interpreter::skip_whitespace() {
    while (isspace(text[pos])) {
        ++pos;
    }
}

int Interpreter::get_number() {
    int ret = 0;
    while (pos < text.size() && isdigit(text[pos])) {
        ret = ret * 10 + text[pos] - '0';
        ++pos;
    }
    return ret;
}

Token Interpreter::get_next_token() {
    while (pos < text.size()) {
        if (isspace(text[pos])) {
            skip_whitespace();
            continue;
        } else if (isdigit(text[pos])) {
            cur_value = get_number();
            return Token::NUMBER;
        } else if (text[pos] == '+') {
            ++pos;
            return Token::PLUS;
        } else if (text[pos] == '-') {
            ++pos;
            return Token::MINUS;
        } else
            error();
    }
    return Token::TEOF;
}

void Interpreter::error() {
    throw std::runtime_error("Parsing error");
}

void Interpreter::eat(Token type) {
    if (type == cur_token)
        cur_token = get_next_token();
    else
        error();
}

int Interpreter::item() {
    int result = cur_value;
    eat(Token::NUMBER);
    return result;
}

int Interpreter::expr() {
    cur_token = get_next_token();

    int result = item();
    while (pos < text.size()) {
        Token token = cur_token;
        if (token == Token::PLUS) {
            eat(Token::PLUS);
            result += item();
        } else if (token == Token::MINUS) {
            eat(Token::MINUS);
            result -= item();
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text.empty() || std::all_of(text.cbegin(), text.cend(), isspace))
            continue;
        Interpreter interpreter(text);
        std::cout << interpreter.expr() << "\ncalc> ";
    }
    std::cout << '\n';
    return 0;
}

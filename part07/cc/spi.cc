#include "Interpreter.h"
#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::string text;
    std::cout << "spi> ";
    while (getline(std::cin, text)) {
        if (text.empty() || std::all_of(text.cbegin(), text.cend(), isspace))
            continue;
        Interpreter interpreter{Parser{Lexer{text}}};
        std::cout << interpreter.interpret() << "\nspi> ";
    }
    std::cout << '\n';
    return 0;
}

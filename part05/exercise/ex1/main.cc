#include "Interpreter.h"
#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char **argv)
{
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text.empty() || std::all_of(text.cbegin(), text.cend(), isspace))
            continue;
        Interpreter parser{Lexer(text)};
        std::cout << parser.parse() << "\ncalc> ";
    }
    std::cout << '\n';

    return 0;
}

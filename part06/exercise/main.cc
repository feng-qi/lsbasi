#include "Interpreter.h"
#include <string>
#include <iostream>
#include <algorithm>

int main(int argc, char **argv)
{
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text.empty() || std::all_of(text.cbegin(), text.cend(), isspace)) {
            continue;
        }
        Interpreter interpreter{Lexer(text)};
        std::cout << interpreter.parse() << "\ncalc> ";
    }
    std::cout << '\n';

    return 0;
}

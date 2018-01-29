#include "parser.h"
#include <algorithm>

int main(int argc, char **argv)
{
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text.empty() || std::all_of(text.cbegin(), text.cend(), isspace))
            continue;
        Parser parser{ Lexer(text) };
        parser.parse();
        std::cout << "calc> ";
    }
    std::cout << '\n';

    return 0;
}

#include <iostream>

#include "antlr4-runtime.h"
#include "CPP14Lexer.h"
#include "CPP14Parser.h"

using namespace cpp_parser;
using namespace antlr4;

int main(int , const char **) {
    ANTLRInputStream input("int main(){}");
    CPP14Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << std::endl;
    }

    CPP14Parser parser(&tokens);
    tree::ParseTree* tree = parser.translationUnit();

   // std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

    return 0;
}
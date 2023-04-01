#include <iostream>

#include "antlr4-runtime.h"
#include "CMakeLexer.h"
#include "CMakeParser.h"

using namespace cmake_parser;
using namespace antlr4;

int main(int , const char **) {
    ANTLRInputStream input("CMAKE_MINIMUM_REQUIRED(VERSION 3.5)");
    CMakeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << std::endl;
    }

    CMakeParser parser(&tokens);
    tree::ParseTree* tree = parser.file_();

   // std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

    return 0;
}
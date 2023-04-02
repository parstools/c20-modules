#include <iostream>
#include <fstream>

#include "antlr4-runtime.h"
#include "tree/ParseTreeWalker.h"
#include "CMakeLexer.h"
#include "CMakeParser.h"
#include "CMakeBaseListener.h"

using namespace cmake_parser;
using namespace antlr4;

class CMakeListnener: public cmake_parser::CMakeBaseListener{
    std::unordered_set <std::string> idents;
    void enterCommand_invocation(CMakeParser::Command_invocationContext *ctx) override {
        idents.insert(ctx->Identifier()->toString());
    }
public:
    void printIdents() {
        for (auto id: idents)
            std::cout << id << std::endl;
    }
};

int main(int , const char **) {
    std::ifstream ifs("../CMakeLists.txt");
    ANTLRInputStream input(ifs);
    CMakeLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
//    for (auto token : tokens.getTokens()) {
//        std::cout << token->toString() << std::endl;
//    }

    CMakeListnener listener;
    CMakeParser parser(&tokens);
    tree::ParseTree* tree = parser.file_();
//    std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    listener.printIdents();
    return 0;
}
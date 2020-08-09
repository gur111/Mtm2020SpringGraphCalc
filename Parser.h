#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "BinTree.h"

namespace GraphCalc {

enum class TokenType {
    INVALID,
    GRAPH_LITERAL,
    GRAPH_NAME,
    FUNCTION_NAME,
    SINGLE_TOKEN,
    FILENAME
};
std::shared_ptr<BinTree> parseLine(std::string line);
std::string getNextToken(const std::string &line = "", bool peak = false,
                         bool expect_filename = false,
                         std::shared_ptr<TokenType> type = nullptr);
}  // namespace GraphCalc

#endif
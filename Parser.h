#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "BinTree.h"

namespace GraphCalc {

std::shared_ptr<BinTree> parseLine(const std::string &line);
std::string getNextToken(const std::string &line = "", bool peak = false,
                         bool expect_filename = false);
}  // namespace GraphCalc

#endif
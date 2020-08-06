#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "BinTree.h"

namespace GraphCalc {

std::string getNextToken(const std::string &line = "", bool peak = false);
std::shared_ptr<BinTree> parseLine(const std::string &line);
}  // namespace GraphCalc

#endif
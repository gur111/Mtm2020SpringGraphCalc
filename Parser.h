#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "BinTree.h"

namespace GraphCalc {

string getNextToken(const string &line = "", bool peak = false);
shared_ptr<BinTree> parseLine(const string &line);
}  // namespace GraphCalc

#endif
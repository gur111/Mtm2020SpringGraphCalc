
#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace GraphCalc {
std::string extractGraphLiteralToken(const std::string &subline);
void sanitizeGraphLiteralToken(std::string &token);
std::string extractFuncParams(std::string command);
}  // namespace GraphCalc

#endif
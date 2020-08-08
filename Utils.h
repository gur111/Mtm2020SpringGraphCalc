
#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace GraphCalc {
std::string extractGraphLiteralToken(const std::string &subline);
void sanitizeGraphLiteralToken(std::string &token);
std::string extractFuncParams(std::string command, bool start_to_end = true);
bool isValidGraphName(const std::string &name);
bool areBracesBalanced(const std::string &line);
}  // namespace GraphCalc

#endif
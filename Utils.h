
#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace GraphCalc {
std::string extractGraphLiteralToken(const std::string &subline);
void sanitizeGraphLiteralToken(std::string &token);
std::string extractFuncParams(std::string command, bool start_to_end = true);
bool isValidGraphName(const std::string &name);
bool isValidNodeName(const std::string &name);
bool areBracesBalanced(const std::string &line, bool verify_semicolon = false);
std::string trim(const std::string &token);
}  // namespace GraphCalc

#endif

#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace GraphCalc {

enum FuncStage {
    NONE,
    PRE_LOAD_OPEN,
    PRE_SAVE_OPEN,
    MID_FILENAME,
    PRE_COMMA,
    SAVE_FIRST_ARG
};

std::string extractGraphLiteralToken(const std::string &subline);
void sanitizeGraphLiteralToken(std::string &token);
std::string extractFuncParams(std::string command, bool start_to_end = true);
bool isValidGraphName(const std::string &name);
bool isValidNodeName(const std::string &name);
bool areBracesBalanced(const std::string &line, bool verify_semicolon = false,
                       FuncStage stage = NONE);
std::string trim(const std::string &token);
}  // namespace GraphCalc

#endif
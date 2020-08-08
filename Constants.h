#include <set>
#include <string>

const std::set<std::string> BIN_OPERATORS = {"+", "-", "*", "^", "="};
const std::set<std::string> UNARY_OPERATORS = {"!"};
const std::set<std::string> RESERVED_WORDS = {"quit", "save",  "load", "delete",
                                              "who",  "reset", "print"};
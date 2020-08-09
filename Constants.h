#include <set>
#include <string>

const std::set<std::string> BIN_OPERATORS = {"+", "-", "*", "^", "="};
const std::set<std::string> UNARY_OPERATORS = {"!"};
const std::set<std::string> RESERVED_WORDS = {"quit", "save",  "load", "delete",
                                              "who",  "reset", "print"};
// The maximum sum of edges and nodes in a graph (to avoid long execution times due to corrupt load files)
const int MAX_ITEMS_IN_GRAPH = 20000;
#include "Utils.h"

#include <regex>

using std::shared_ptr;
using std::string;

namespace GraphCalc {
string extractGraphLiteralToken(const string &subline) {
    std::smatch matches;
    string name_prefix = "\\s*([A-z\\[])";
    string name_chars = "[A-z0-9;\\[\\]]";
    string node_regex = "(" + name_prefix + name_chars + "*)\\s*";
    string edge_regex = "\\s*<" + node_regex + "," + node_regex + ">\\s*";
    std::regex regexp("^\\{(" + node_regex + ",)*" + node_regex + "(\\|" +
                      edge_regex + "(," + edge_regex + ")*)?\\}");
    std::regex_search(subline, matches, regexp);
    if (matches.size() == 0) {
        return "";
    }
    return matches[0];
}

void sanitizeGraphLiteralToken(string &token) {
    token.erase(remove_if(token.begin(), token.end(), isspace), token.end());
}

string extractFuncParams(string command) {
    std::smatch matches;
    std::regex match_func_format("^[A-z]*\\s*\\(.*\\)\\s*$");
    if (std::regex_search(command, matches, match_func_format)) {
        int spos = command.find("("), epos = command.rfind(")");
        return command.substr(spos + 1, epos - spos - 2);
    }
    // Format mismatch
    // TODO: Throw exception?
    return "";
}

}  // namespace GraphCalc
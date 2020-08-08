#include "Utils.h"

#include <regex>

#include "Constants.h"
#include "Exceptions.h"

using std::shared_ptr;
using std::string;
using std::vector;

namespace GraphCalc {
string extractGraphLiteralToken(const string &subline) {
    std::smatch matches;
    string name_prefix = "\\s*([A-z\\[])";
    string name_chars = "[A-z0-9;\\[\\]]";
    string node_regex = "(" + name_prefix + name_chars + "*)\\s*";
    string edge_regex = "\\s*<" + node_regex + "," + node_regex + ">\\s*";
    std::regex regexp("^\\{\\s*((" + node_regex + ",)*" + node_regex +
                      ")?(\\|(" + edge_regex + "(," + edge_regex +
                      ")*)?)?\\s*\\}");
    std::regex_search(subline, matches, regexp);
    if (matches.size() == 0) {
        return "";
    }
    return matches[0];
}
bool areBracesBalanced(const string &line) {
    vector<char> braces_stack;
    for (unsigned int i = 0; i < line.length(); i++) {
        switch (line[i]) {
            case '(':
            case '{':
            case '[':
                braces_stack.push_back(line[i]);
                break;
            case ')':
                if (braces_stack.size() == 0 or braces_stack.back() != '(') {
                    return false;
                }
                braces_stack.pop_back();
                break;
            case '}':
            case ']':
                if (braces_stack.size() == 0 or
                    line[i] - 2 != braces_stack.back()) {
                    return false;
                }
                braces_stack.pop_back();
                break;
        }
    }

    return braces_stack.size() == 0;
}

bool isValidGraphName(const string &name) {
    if (RESERVED_WORDS.find(name) != RESERVED_WORDS.end()) {
        return false;
    }

    std::smatch matches;
    string name_prefix = "([A-z])";
    string name_chars = "[A-z0-9]";
    std::regex name_regex("^(" + name_prefix + name_chars + "*)$");
    if (not std::regex_search(name, matches, name_regex)) {
        return false;
    }
    return true;
}

void sanitizeGraphLiteralToken(string &token) {
    token.erase(remove_if(token.begin(), token.end(), isspace), token.end());
}

string trim(const string &token) {
    string delims = "\t ";
    size_t begin_pos = token.find_first_not_of(delims);
    if (begin_pos == std::string::npos) {
        return "";
    }

    return token.substr(begin_pos,
                        token.find_last_not_of(delims) - begin_pos + 1);
}
string extractFuncParams(string command, bool start_to_end) {
    std::smatch matches;
    string base_exp = "[A-z]*\\s*\\(.*\\)\\s*";
    std::regex match_func_format(start_to_end ? ("^" + base_exp + "$")
                                              : base_exp);
    if (std::regex_search(command, matches, match_func_format)) {
        int spos = command.find("("), epos = command.rfind(")");
        return command.substr(spos + 1, epos - spos - 1);
    }
    // Format mismatch
    throw SyntaxError("Function call is in an invalid format");
}

}  // namespace GraphCalc
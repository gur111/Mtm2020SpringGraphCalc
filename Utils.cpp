#include "Utils.h"

#include <regex>

#include "Constants.h"
#include "Exceptions.h"

using std::shared_ptr;
using std::string;
using std::vector;

namespace GraphCalc {
string node_name_chars_regex_str = "[A-z0-9;\\[\\]]";
string node_regex_str = "(" + node_name_chars_regex_str + "+)";

string extractGraphLiteralToken(const string &subline) {
    std::smatch matches;
    string node_regex = "\\s*" + node_regex_str + "\\s*";
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
bool areBracesBalanced(const string &line, bool is_node_name, FuncStage stage) {
    vector<char> stack;
    size_t func_start_stack_height = 0;
    for (unsigned int i = 0; i < line.length(); i++) {
        if (not is_node_name and stage == NONE and
            std::count(stack.begin(), stack.end(), '{') == 0) {
            if (line.substr(i).find("load") == 0 or
                line.substr(i).find("save") == 0) {
                if (i > 0 and isalnum(line[i - 1])) {
                    continue;
                }
                if (line.find("save") == i) {
                    stage = PRE_SAVE_OPEN;
                    func_start_stack_height = stack.size();
                } else {
                    stage = PRE_LOAD_OPEN;
                    func_start_stack_height = stack.size();
                }
                i += 3;
                continue;
            }
        }
        switch (line[i]) {
            case '{':
                if (std::count(stack.begin(), stack.end(), '{') != 0) {
                    throw SyntaxError(
                        "Only one level of curly braces is allowed.");
                }
            case '[':
                // Ignore mid filename
                if (stage == MID_FILENAME) {
                    break;
                } else if (stage == PRE_LOAD_OPEN or stage == PRE_SAVE_OPEN) {
                    return false;
                }
            case '(':
                if (stage == PRE_LOAD_OPEN) {
                    stage = MID_FILENAME;
                } else if (stage == PRE_SAVE_OPEN) {
                    stage = SAVE_FIRST_ARG;
                }
                stack.push_back(line[i]);
                break;
            case ')':
                if (stage == MID_FILENAME) {
                    stage = NONE;
                }
                if (stack.size() == 0 or stack.back() != '(') {
                    return false;
                }
                stack.pop_back();
                break;
            case '}':
            case ']':
                if (stage == MID_FILENAME) {
                    break;
                }
                if (stack.size() == 0 or line[i] - 2 != stack.back()) {
                    return false;
                }
                stack.pop_back();
                break;
            case ';':
                if (stage == MID_FILENAME) {
                    break;
                }
                if (is_node_name and
                    count(stack.begin(), stack.end(), '[') == 0) {
                    return false;
                }
                break;
            case ' ':
                break;
            default:
                // If a comma found on the same level (braces wise) as the
                // function openning "("
                if (line[i] == ',' and stage == SAVE_FIRST_ARG and
                    stack.size() == (func_start_stack_height + 1) and
                    stack.back() == '(') {
                    stage = MID_FILENAME;
                    break;
                }
                if ((stage == PRE_LOAD_OPEN or stage == PRE_SAVE_OPEN)) {
                    stage = NONE;
                }
        }
    }

    return stack.size() == 0;
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
bool isValidNodeName(const string &name) {
    std::smatch matches;
    std::regex node_regex(node_regex_str);
    if (not std::regex_search(name, matches, node_regex)) {
        return false;
    }

    return areBracesBalanced(name, true);
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
        string result = command.substr(spos + 1, epos - spos - 1);
        return result;
    }
    // Format mismatch
    throw SyntaxError("Function call is in an invalid format");
}

}  // namespace GraphCalc
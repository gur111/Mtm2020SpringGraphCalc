#include "Parser.h"

#include <cctype>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#define pairBO pair<shared_ptr<BinTree>, string>

using std::cout;
using std::endl;
using std::pair;
using std::set;
using std::vector;

namespace GraphCalc {

shared_ptr<BinTree> parseLine(const string &line) {
    /* This will parse a line of input with an algorithm based on the
     Shunting-yard algorithm and a parse/syntax tree concept */
    enum class Stage { DEFAULT, LOAD_ARG, LOAD_ARG_R };
    shared_ptr<BinTree> tree_root(new BinTree(""));
    vector<pairBO> tree_stack;
    shared_ptr<BinTree> curr_tree = tree_root;
    tree_stack.push_back(pairBO(curr_tree, ""));
    string token;
    Stage stage = Stage::DEFAULT;

    // TODO: Handle function calls which aren't `load`
    token = getNextToken(line);

    // Handle mutable expression
    while ((token = getNextToken()) != "") {
        switch (stage) {
            // If outside a `load` function
            case Stage::DEFAULT:
                if (token == "(") {
                    curr_tree->createLeft("");
                    tree_stack.push_back(pairBO(curr_tree, ""));
                    curr_tree = curr_tree->getLeft();
                } else if (token == ")") {
                    if (tree_stack.back().first == tree_root) {
                        // TODO: Throw unbalanced expression. Couln't find
                        // matching openning bracket
                    }
                    curr_tree = tree_stack.back().first;
                    tree_stack.pop_back();
                } else if (BIN_OPERATORS.find(token) != BIN_OPERATORS.end()) {
                    // Handle binary operators
                    curr_tree->set(token);
                    curr_tree->createRight("");
                    tree_stack.push_back(pairBO(curr_tree, ""));
                    curr_tree = curr_tree->getRight();
                } else if (UNARY_OPERATORS.find(token) !=
                           UNARY_OPERATORS.end()) {
                    // Handle unary operators
                    curr_tree->createLeft("!");
                    tree_stack.push_back(pairBO(curr_tree, token));
                    curr_tree = curr_tree->getLeft()->createLeft("");
                } else if (token == "load") {
                    // Handle inline load
                    curr_tree->createLeft("load");
                    tree_stack.push_back(pairBO(curr_tree, token));
                    curr_tree = curr_tree->getLeft()->createLeft("");
                    stage = Stage::LOAD_ARG;
                    if ((token = getNextToken()) != "(") {
                        // TODO: Throw syntax error after function call.
                        // Expected
                        // "(" got `token`
                    }
                } else {
                    string tmp_token = getNextToken("", true);
                    if (tmp_token != "" and tree_stack.back().second == "" and
                        BIN_OPERATORS.find(tmp_token) != BIN_OPERATORS.end()) {
                        curr_tree->set(tmp_token);
                        curr_tree->createLeft(token);
                        curr_tree->createRight("");
                        curr_tree = curr_tree->getRight();
                        getNextToken();
                    } else {
                        curr_tree->set(token);
                        curr_tree = tree_stack.back().first;
                        tree_stack.pop_back();
                    }
                }
                break;
            // Read left `load` arg
            case Stage::LOAD_ARG:
                // TODO: Verify func left arg. Probably just a var name
                curr_tree->set(token);
                curr_tree = tree_stack.back().first;
                tree_stack.pop_back();
                stage = Stage::DEFAULT;
                if ((token = getNextToken()) != ")") {
                    // TODO: Throstartposw syntax error after function
                    // arg. Expected
                    // ")" got `token`
                }
                break;
            default:
                // TODO: Throw unknown stage
                break;
        }
    }
    if (tree_stack.back().first != tree_root) {
        cout << "Error: Unbalanced at the end" << endl;
        // TODO: Throw unbalanced expression. No matching closing bracket
    }
    tree_root->cleanup();
    return tree_root;
}

bool isNodeNameLegal(const string &name) {
    // TODO: Implement
    return true;
}

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

string getNextToken(const string &line, bool peak) {
    static string cur_line;
    static int pos = 0;
    const string single_char_tokens = "()!*-+^=";

    enum class TokenType {
        UNKNOWN,
        OPERATOR,
        FUNC_NAME,
        VAR_NAME,
        GRAPH_LITERAL,
        COMMA,
        BRACES
    };
    int tmp_pos = pos;
    TokenType type = TokenType::UNKNOWN;

    // Init static(s) if a new line if given
    if (not line.empty()) {
        // Only init. Don't return any tokens
        pos = 0;
        cur_line = line;
        return "";
    }

    // Skip blank chars
    while (isblank(cur_line[tmp_pos])) {
        tmp_pos++;
    }

    // If end of the line
    if (cur_line[tmp_pos] == '\0') {
        if (not peak) pos = tmp_pos;
        return "";
    }

    // Detect single char tokens
    if (single_char_tokens.find(cur_line[tmp_pos]) != string::npos) {
        tmp_pos++;
        if (not peak) pos = tmp_pos;
        return cur_line.substr(tmp_pos - 1, 1);
    }

    // Handle graph literals
    if (cur_line[tmp_pos] == '{') {
        int startpos = tmp_pos;
        string match = extractGraphLiteralToken(cur_line.substr(startpos));
        if (match == "") {
            cout << "Error: invalid graph literal format at char: " << tmp_pos
                 << endl;
            return "";  // TODO: Throw error
        }
        if (not peak) pos = tmp_pos + match.length();
        sanitizeGraphLiteralToken(match);
        /*Return the sanitized token */
        return match;
    }

    // Detect func/var name
    if (isalpha(cur_line[tmp_pos])) {
        int startpos = tmp_pos;
        while (isalnum(cur_line[++tmp_pos]))
            ;
        if (not peak) pos = tmp_pos;
        return cur_line.substr(startpos, tmp_pos - startpos);
    }
    // Unknown token type
    else {
        // TODO: Throw exception
        return "";
    }
}

};  // namespace GraphCalc

#include "Parser.h"

#include <cctype>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Exceptions.h"
#include "Utils.h"
#define pairBO pair<shared_ptr<BinTree>, string>

using std::cout;
using std::endl;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

namespace GraphCalc {
string addBraces(string line);

shared_ptr<BinTree> parseLine(string line) {
    /* This will parse a line of input with an algorithm based on the
     Shunting-yard algorithm and a parse/syntax tree concept */
    shared_ptr<BinTree> tree_root(new BinTree(""));
    vector<pairBO> tree_stack;
    shared_ptr<BinTree> curr_tree = tree_root;
    tree_stack.push_back(pairBO(curr_tree, ""));
    string token;
    shared_ptr<TokenType> type(new TokenType());
    bool was_last_graph = false;

    if (not areBracesBalanced(line)) {
        throw SyntaxError("Braces unbalanced.");
    }

    getNextToken(line, false, false, type);

    string tmp_token = getNextToken("", true);

    if (tmp_token == "print") {
        curr_tree->set(tmp_token);
        tree_stack.pop_back();
        tree_stack.push_back(
            pairBO((curr_tree = curr_tree->createLeft("")), ""));
        line = extractFuncParams(line);
        if (not areBracesBalanced(line)) {
            throw SyntaxError("Invalid print parameter: " + line);
        }
        line = addBraces(line);
        getNextToken(line, false, false, type);
    } else if (tmp_token == "delete") {
        curr_tree->set(tmp_token);
        tmp_token = extractFuncParams(line);
        curr_tree->createLeft(tmp_token);
        getNextToken(tmp_token, false, false, type);
        getNextToken();  // Discard first token (we already have it)
        // Make sure only one parameter
        if ((tmp_token = getNextToken()) != "") {
            throw SyntaxError("Unexpected: " + tmp_token +
                              " in \"delete\" command.");
        }
        return tree_root;
    } else if (tmp_token == "save") {
        curr_tree->set(tmp_token);
        tree_stack.pop_back();
        string first_param =
            extractFuncParams(line) + ")";  // Must have ")" to detect filename

        size_t comma_pos = first_param.rfind(",");
        if (comma_pos == string::npos) {
            throw SyntaxError("Missing comma.");
        }
        getNextToken(first_param.substr(comma_pos + 1), false, false, type);
        token = getNextToken("", false, true);
        if (token == "") {
            throw SyntaxError("Failed to detect filename.");
        }
        // Discard ")"
        if ((tmp_token = getNextToken()) != ")") {
            throw SyntaxError("Expected \")\" but got \"" + tmp_token + "\".");
        }
        curr_tree->createRight(token);
        // Make sure there's no more data (for example if we had something like
        // this: "save(G1, asdf)G2)")
        if ((tmp_token = getNextToken()) != "") {
            throw SyntaxError(tmp_token +
                              " unexpected after \"save\" function call.");
        }
        tree_stack.push_back(pairBO(curr_tree->createLeft(""), ""));
        curr_tree = curr_tree->getLeft();
        // Add braces to avoid ambiguity
        line = addBraces(first_param.substr(0, comma_pos));
        getNextToken(line, false, false, type);
    } else if (tmp_token == "who" or tmp_token == "quit" or
               tmp_token == "reset") {
        // Now read it for realsies
        curr_tree->set(getNextToken());
        // Make sure no more tokens
        if ((tmp_token = getNextToken()) != "") {
            throw SyntaxError("Unexpected: " + tmp_token + " after command.");
        }
        return tree_root;
    } else {
        line = addBraces(line);
        getNextToken(line, false, false, type);
    }

    // Handle mutable expression
    while ((token = getNextToken()) != "") {
        if (token == "(") {
            if (curr_tree == tree_root) {
                throw SyntaxError("Command cannot start with braces.");
            }
            curr_tree->createLeft("");
            tree_stack.push_back(pairBO(curr_tree, ""));
            curr_tree = curr_tree->getLeft();
        } else if (token == ")") {
            if (curr_tree == tree_root) {
                throw SyntaxError("Matching openning bracket not detected.");
            }
            if (tree_stack.size() == 0) {
                throw SyntaxError("Failed to parse line from: " + token);
            }
            curr_tree = tree_stack.back().first;
            tree_stack.pop_back();
            if (tree_stack.size() > 0 and tree_stack.back().second != "") {
                curr_tree = tree_stack.back().first;
                tree_stack.pop_back();
            }
        } else if (BIN_OPERATORS.find(token) != BIN_OPERATORS.end()) {
            was_last_graph = false;
            // Handle binary operators
            curr_tree->set(token);
            curr_tree = curr_tree->createRight("");
        } else if (UNARY_OPERATORS.find(token) != UNARY_OPERATORS.end()) {
            // Handle unary operatorsUNARY_OPERATORS.end()) {
            // Handle unary operators
            tree_stack.push_back(pairBO(curr_tree, token));
            curr_tree = curr_tree->createLeft(token)->createLeft("");
        } else if (*type == TokenType::GRAPH_LITERAL or
                   *type == TokenType::GRAPH_NAME or
                   *type == TokenType::FUNCTION_NAME) {
            if (was_last_graph) {
                throw SyntaxError("Missing operand between tokens.");
            }
            was_last_graph = true;
            string tmp_token = getNextToken("", true);
            // Check if the next token is a binary operator
            if (tmp_token != "" and tree_stack.back().second == "" and
                BIN_OPERATORS.find(tmp_token) != BIN_OPERATORS.end()) {
                was_last_graph = false;
                curr_tree->set(tmp_token);
                if (*type == TokenType::FUNCTION_NAME) {
                    curr_tree->createLeft("load")->createLeft(
                        token.substr(5, token.length() - 6));
                } else {
                    curr_tree->createLeft(token);
                }
                curr_tree = curr_tree->createRight("");
                getNextToken();
            } else {
                if (*type == TokenType::FUNCTION_NAME) {
                    if (token.find("load") != 0) {
                        throw SyntaxError("Unexpected reserved word: " + token);
                    }
                    curr_tree->set("load");
                    curr_tree->createLeft(token.substr(5, token.length() - 6));
                } else {
                    curr_tree->set(token);
                }
                if (tree_stack.size() == 0) {
                    throw SyntaxError("Failed to parse line after " + token);
                }
                if (tree_stack.back().second != "") {
                    curr_tree = tree_stack.back().first;
                    tree_stack.pop_back();
                }
            }
        } else {
            throw SyntaxError("Token: " + token);
        }
    }
    if (not tree_root->isLeaf()) {
        if (tree_root->get() == "") {
            throw SyntaxError("Unknown syntax.");
        }
        tree_root->cleanup();
    }

    // Uncomment this line to unleash hell (JK, it merely prints the tree in a
    // very convinient way)
    // cout << tree_root;

    return tree_root;
}

/**
 * This adds braces to the input line so there's not a sequence of 2 operations
 * in a row.
 */
string addBraces(string line) {
    vector<char> braces_stack;
    int braces_count = -1;
    size_t braces_pos = 0;
    vector<pair<size_t, int>> load_ranges;

    {  // Mark loads for skipping
        std::regex pattern("(load\\s*\\([^\\)\\(]*\\))");

        std::sregex_iterator t(line.begin(), line.end(), pattern);
        auto line_end = std::sregex_iterator();

        while (t != line_end) {
            load_ranges.push_back(
                pair<size_t, int>(t->position(), (*t)[0].length()));
            ++t;
        }
    }

    for (size_t i = 0; i < line.length(); i++) {
        // Skip load ranges
        if (load_ranges.size() > 0 and i == load_ranges[0].first) {
            i += load_ranges[0].second - 1;
            load_ranges.erase(load_ranges.begin());
            continue;
        }

        // TODO: Ignore operators while mid filename
        if (line[i] == '(') {
            size_t startpos = i;
            braces_stack.push_back(')');
            while (braces_stack.size() != 0) {
                switch (line[++i]) {
                    case '(':
                        braces_stack.push_back(')');
                        break;
                    case ')':
                        braces_stack.pop_back();
                        break;
                }
            }
            string orig_sub = line.substr(startpos + 1, i - startpos - 1);
            string sub_result = addBraces(orig_sub);
            line = line.substr(0, startpos + 1) + sub_result + line.substr(i);
            // We need to advance i by the size we added to the string
            // (sub_result)
            if (sub_result.length() - orig_sub.length() < 0) {
                throw Unknown(
                    "Unexpected error occured. When adding braces, sub result "
                    "shorter than original sub string.");
            }
            i += sub_result.length() - orig_sub.length();
        } else if (BIN_OPERATORS.find(string(1, line[i])) !=
                   BIN_OPERATORS.end()) {
            if (line[i] == '=') {
                braces_pos = i + 1;
                continue;
            }
            braces_count++;
            if (braces_count != 0) {
                line = line.substr(0, i) + ")" + line.substr(i);
                // Now we need to advance i to skip the closing brace
                i++;
            }
        }
        // else if (line.find("load") == i and
        //            // and not in curly braces
        //            line.substr(0, i).find_last_of('{') >
        //                line.substr(0, i).find_last_of('}')) {
        //     if ((i = line.find(")")) == string::npos) {
        //         throw SyntaxError(
        //             "Got weird after load. Expecting \")\" somewhere.");
        //     }
        // }
    }
    return line.substr(0, braces_pos) +
           string(braces_count > 0 ? braces_count : 0, '(') +
           line.substr(braces_pos);
}

string getNextToken(const string &line, bool peak, bool expect_filename,
                    shared_ptr<TokenType> tok_type) {
    static string cur_line;
    static int pos = 0;
    static shared_ptr<TokenType> type;
    const string single_char_tokens = "()!*-+^=";

    int tmp_pos = pos;

    // Init static(s) if a new line if given
    if (not line.empty()) {
        // Only init. Don't return any tokens
        pos = 0;
        cur_line = line;
        type = tok_type;
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

    // Match filename
    if (expect_filename) {
        int startpos = tmp_pos;
        if (cur_line[tmp_pos] == ')') {
            throw Missing("Missing filename.");
        }
        while (cur_line[++tmp_pos] != ')') {
            if (cur_line[tmp_pos] == '\0') {
                throw SyntaxError("Expected \")\" after filename.");
            }
            if (cur_line[tmp_pos] == ',' or cur_line[tmp_pos] == '(') {
                throw SyntaxError("Filename cannot include commas or braces.");
            }
        }
        if (not peak) pos = tmp_pos;
        if (type != nullptr and not peak) {
            *type = TokenType::FILENAME;
        }
        return trim(cur_line.substr(startpos, tmp_pos - startpos));
    }

    // Detect single char tokens
    if (single_char_tokens.find(cur_line[tmp_pos]) != string::npos) {
        tmp_pos++;
        if (not peak) pos = tmp_pos;
        if (type != nullptr and not peak) {
            *type = TokenType::SINGLE_TOKEN;
        }
        return cur_line.substr(tmp_pos - 1, 1);
    }

    // Handle graph literals
    if (cur_line[tmp_pos] == '{') {
        int startpos = tmp_pos;
        string match = extractGraphLiteralToken(cur_line.substr(startpos));
        if (match == "") {
            throw SyntaxError("Graph literal format: " +
                              cur_line.substr(startpos));
        }
        if (not peak) pos = tmp_pos + match.length();
        sanitizeGraphLiteralToken(match);
        if (type != nullptr and not peak) {
            *type = TokenType::GRAPH_LITERAL;
        }
        /*Return the sanitized token */
        return match;
    }

    // Detect func/var name
    if (isalpha(cur_line[tmp_pos])) {
        int startpos = tmp_pos;
        while (isalnum(cur_line[++tmp_pos]))
            ;
        if (not peak) pos = tmp_pos;

        string tmp_token = cur_line.substr(startpos, tmp_pos - startpos);

        int backup_pos = pos;
        pos = tmp_pos;

        if (tmp_token == "load") {
            // Don't peak (pos was backed up, will be restored before return)
            if ((tmp_token = getNextToken("", false)) != "(") {
                throw SyntaxError("Got token " + tmp_token +
                                  " after function call");
            } else if ((tmp_token = getNextToken("", false, true)) == "") {
                throw SyntaxError("Failed to detect filename for load.");
            } else if (getNextToken("", false) != ")") {
                throw SyntaxError("Token after " + tmp_token);
            }
            tmp_token = "load(" + tmp_token + ")";
            if (type != nullptr and not peak) {
                *type = TokenType::FUNCTION_NAME;
            }
        } else if (type != nullptr and not peak) {
            if (isValidGraphName(
                    cur_line.substr(startpos, tmp_pos - startpos))) {
                *type = TokenType::GRAPH_NAME;
            } else if (RESERVED_WORDS.find(tmp_token) != RESERVED_WORDS.end()) {
                *type = TokenType::FUNCTION_NAME;
            } else {
                *type = TokenType::INVALID;
            }
        }

        if (peak) {
            pos = backup_pos;
        }

        return tmp_token;
    }
    // Unknown token type
    else {
        throw SyntaxError("Unknown token type: " + cur_line.substr(tmp_pos));
    }
}

}  // namespace GraphCalc

#include "Parser.h"

#include <cctype>
#include <iostream>
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

shared_ptr<BinTree> parseLine(const string &line) {
    /* This will parse a line of input with an algorithm based on the
     Shunting-yard algorithm and a parse/syntax tree concept */
    shared_ptr<BinTree> tree_root(new BinTree(""));
    vector<pairBO> tree_stack;
    shared_ptr<BinTree> curr_tree = tree_root;
    tree_stack.push_back(pairBO(curr_tree, ""));
    string token;
    shared_ptr<TokenType> type(new TokenType());

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
        getNextToken(extractFuncParams(line), false, false, type);
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
        // tmp_token[comma_pos] = '+';  // Replace comma with an operator
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
        getNextToken(first_param.substr(0, comma_pos), false, false, type);
    } else if (tmp_token == "who" or tmp_token == "quit" or
               tmp_token == "reset") {
        // Now read it for realsies
        curr_tree->set(getNextToken());
        // Make sure no more tokens
        if ((tmp_token = getNextToken()) != "") {
            throw SyntaxError("Unexpected: " + tmp_token + " after command.");
        }
        return tree_root;
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
            // if (tree_stack.back().first == tree_root) {
            if (curr_tree == tree_root) {
                throw SyntaxError("Matching openning bracket not detected.");
            }
            if (tree_stack.size() == 0) {
                throw SyntaxError("Failed to parse line from: " + token);
            }
            curr_tree = tree_stack.back().first;
            tree_stack.pop_back();
        } else if (BIN_OPERATORS.find(token) != BIN_OPERATORS.end()) {
            // Handle binary operators
            curr_tree->set(token);
            tree_stack.push_back(pairBO(curr_tree->createRight(""), ""));
            curr_tree = curr_tree->getRight();
        } else if (UNARY_OPERATORS.find(token) != UNARY_OPERATORS.end()) {
            // Handle unary operatorsUNARY_OPERATORS.end()) {
            // Handle unary operators
            curr_tree->createLeft("!");
            tree_stack.push_back(pairBO(curr_tree, token));
            curr_tree = curr_tree->getLeft()->createLeft("");
        } else if (token == "load") {
            // Handle inline load
            if ((token = getNextToken()) != "(") {
                throw SyntaxError("Got token " + token +
                                  " after function call");
            }
            curr_tree->createLeft("load");
            token = getNextToken("", false, true);
            if (token == "") {
                throw SyntaxError("Failed to detect filename.");
            }
            if (getNextToken() != ")") {
                throw SyntaxError("Token after " + token);
            }
            curr_tree->getLeft()->createLeft(token);
        } else if (*type == TokenType::GRAPH_LITERAL ||
                   *type == TokenType::GRAPH_NAME) {
            string tmp_token = getNextToken("", true);
            if (tmp_token != "" and tree_stack.back().second == "" and
                BIN_OPERATORS.find(tmp_token) != BIN_OPERATORS.end()) {
                curr_tree->set(tmp_token);
                curr_tree->createLeft(token);
                // curr_tree->createRight("");
                curr_tree = curr_tree->createRight("");
                getNextToken();
            } else {
                curr_tree->set(token);
                if (tree_stack.size() == 0) {
                    throw SyntaxError("Failed to parse line after " + token);
                }
                curr_tree = tree_stack.back().first;
                tree_stack.pop_back();
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

    return tree_root;
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
        while (cur_line[++tmp_pos] != ')') {
            if (cur_line[tmp_pos] == '\0') {
                throw SyntaxError("Expected \")\" after filename");
            }
            if (cur_line[tmp_pos] == ',' or cur_line[tmp_pos] == '(') {
                throw SyntaxError("Filename cannot include commas or braces");
            }
        }
        if (not peak) pos = tmp_pos;
        if (type != nullptr) {
            *type = TokenType::FILENAME;
        }
        return cur_line.substr(startpos, tmp_pos - startpos);
    }

    // Detect single char tokens
    if (single_char_tokens.find(cur_line[tmp_pos]) != string::npos) {
        tmp_pos++;
        if (not peak) pos = tmp_pos;
        if (type != nullptr) {
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
        if (type != nullptr) {
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
        if (type != nullptr and
            isValidGraphName(cur_line.substr(startpos, tmp_pos - startpos))) {
            *type = TokenType::GRAPH_NAME;
        }
        return cur_line.substr(startpos, tmp_pos - startpos);
    }
    // Unknown token type
    else {
        throw SyntaxError("Unknown token type: " + cur_line.substr(tmp_pos));
    }
}

}  // namespace GraphCalc

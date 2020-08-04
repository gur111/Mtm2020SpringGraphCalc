#include "Parser.h"

#include <cctype>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

using std::set;
using std::vector;

namespace GraphCalc {

shared_ptr<BinTree> parseLine(const string &line) {
    /* This will parse a line of input with an algorithm based on the
     Shunting-yard algorithm and a parse/syntax tree concept */
    enum class Stage { DEFAULT, LOAD_ARG_L, LOAD_ARG_R };
    const set<string> bin_operators = {"+", "-", "*", "^", "="};
    const set<string> unary_operators = {"!"};
    shared_ptr<BinTree> tree_root(new BinTree(""));
    vector<shared_ptr<BinTree>> tree_stack;
    shared_ptr<BinTree> curr_tree = tree_root;
    tree_stack.push_back(curr_tree);
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
                    tree_stack.push_back(curr_tree);
                    curr_tree = curr_tree->getLeft();
                } else if (token == ")") {
                    if (tree_stack.back() == tree_root) {
                        // TODO: Throw unbalanced expression. Couln't find
                        // matching openning bracket
                    }
                    curr_tree = tree_stack.back();
                    tree_stack.pop_back();
                    string tmp_token = getNextToken("", true);
                    // if (curr_tree->get() == "" and
                    //     bin_operators.find(tmp_token) == bin_operators.end())
                    //     { curr_tree->set(curr_tree->getLeft()->get());
                    //     curr_tree->setLeft(curr_tree->getLeft()->getLeft());
                    //     curr_tree->setRight(curr_tree->getLeft()->getRight());
                    // }
                } else if (bin_operators.find(token) != bin_operators.end()) {
                    curr_tree->set(token);
                    curr_tree->createRight("");
                    tree_stack.push_back(curr_tree);
                    curr_tree = curr_tree->getRight();
                } else if (unary_operators.find(token) !=
                           unary_operators.end()) {
                    curr_tree->set(token);
                    curr_tree->createLeft("");
                    tree_stack.push_back(curr_tree);
                    curr_tree = curr_tree->getLeft();
                } else if (token == "load") {
                    // Handle inline load
                    curr_tree->set(token);
                    curr_tree->createLeft("");
                    tree_stack.push_back(curr_tree);
                    curr_tree = curr_tree->getLeft();
                    stage = Stage::LOAD_ARG_L;
                    if ((token = getNextToken()) != "(") {
                        // TODO: Throw syntax error after function call.
                        // Expected
                        // "(" got `token`
                    }
                } else {
                    string tmp_token = getNextToken("", true);
                    if (tmp_token != "" and
                        bin_operators.find(tmp_token) != bin_operators.end()) {
                        curr_tree->set(tmp_token);
                        curr_tree->createLeft(token);
                        curr_tree->createRight("");
                        tree_stack.push_back(curr_tree);
                        curr_tree = curr_tree->getRight();
                        getNextToken();
                    } else {
                        curr_tree->set(token);
                        curr_tree = tree_stack.back();
                        tree_stack.pop_back();
                    }
                    // TODO: Unexpected token. Maybe var name?
                }
                break;
            // Read left `load` arg
            case Stage::LOAD_ARG_L:
                // TODO: Verify func left arg. Probably just a var name
                curr_tree->set(token);
                curr_tree = tree_stack.back();
                tree_stack.pop_back();
                stage = Stage::LOAD_ARG_R;
                if ((token = getNextToken()) != ",") {
                    // TODO: Throw syntax error after function arg.
                    // Expected
                    // "," got `token`
                }
                break;
            // Read right `load` arg
            case Stage::LOAD_ARG_R:
                // TODO: Verify func left arg. Probably just a var name
                curr_tree->set(token);
                curr_tree = tree_stack.back();
                tree_stack.pop_back();
                stage = Stage::DEFAULT;
                if ((token = getNextToken()) != ")") {
                    // TODO: Throstartposw syntax error after function arg.
                    // Expected
                    // ")" got `token`
                }
                break;
            default:
                // TODO: Throw unknown stage
                break;
        }
    }
    if (tree_stack.back() != tree_root) {
        std::cout << "ERROR: Unbalanced at the end" << std::endl;
        // TODO: Throw unbalanced expression. No matching closing bracket
    }
    tree_root->cleanup();
    return tree_root;
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
        int startpos = tmp_pos;  // TODO: Temp value
        // TODO: Sanitize graph literal and read until '}'
        if (not peak) pos = tmp_pos;
        return cur_line.substr(
            startpos, tmp_pos - startpos) /*Return the sanitized token */;
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

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../Parser.h"

using std::string;
using std::vector;
using namespace GraphCalc;

// Helper functions
shared_ptr<BinTree> getNotTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("!");
    tree->getRight()->createLeft("G2");
    return tree;
}

shared_ptr<BinTree> getBasicLoadTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("load")->createLeft("filename");
    return tree;
}

shared_ptr<BinTree> getBasicLiteralTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("{x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}");
    return tree;
}
shared_ptr<BinTree> getPlusTwoLiteralsTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("+")->createRight(
        "{x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}");
    tree->getRight()->createLeft("{x3,x4,x5|<x3,x4>,<x3,x5>,<x4,x3>,<x5,x4>}");
    return tree;
}

shared_ptr<BinTree> getNotOnBracesTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("+");
    tree->getRight()->createLeft("!")->createLeft("+")->createLeft("G2");
    tree->getRight()->getLeft()->getLeft()->createRight("G3");
    tree->getRight()->createRight("!")->createLeft("G4");
    return tree;
}

shared_ptr<BinTree> getDefaultTree() {
    shared_ptr<BinTree> tree(new BinTree("="));
    tree->createLeft("G1");
    tree->createRight("+");
    tree->getRight()->createLeft("G2");
    tree->getRight()->createRight("^");
    tree->getRight()->getRight()->createLeft("*");
    tree->getRight()->getRight()->getLeft()->createLeft("G3");
    tree->getRight()->getRight()->getLeft()->createRight("G4");
    tree->getRight()->getRight()->createRight("-");
    tree->getRight()->getRight()->getRight()->createLeft("G5");
    tree->getRight()->getRight()->getRight()->createRight("G6");
    return tree;
}
shared_ptr<BinTree> getDefaultAndNotTree() {
    shared_ptr<BinTree> tree = getDefaultTree();
    tree->getRight()->getLeft()->set("!");
    tree->getRight()->getLeft()->createLeft("G2");
    return tree;
}
shared_ptr<BinTree> getDefaultAndLoadTree() {
    shared_ptr<BinTree> tree = getDefaultTree();
    tree->getRight()->getLeft()->set("load");
    tree->getRight()->getLeft()->createLeft("filename");
    return tree;
}
shared_ptr<BinTree> getDefaultLoadAndLiteralTree() {
    shared_ptr<BinTree> tree = getDefaultTree();
    tree->getRight()->getLeft()->set("load");
    tree->getRight()->getLeft()->createLeft("filename");
    tree->getRight()->getRight()->getRight()->getLeft()->set(
        "{x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}");
    return tree;
}

// Fixtures

struct SimpleValidExpressionTest : testing::Test {
    string input;
    SimpleValidExpressionTest() {
        input = "     G1   =G2 + ((G3* G4) ^ (G5     -\tG6))";
    }
};

struct expression_state {
    string expression;
    vector<string> expected_tokens;
    shared_ptr<BinTree> expected_tree;
};

struct ValidExpressionTest : SimpleValidExpressionTest,
                             testing::WithParamInterface<expression_state> {
    ValidExpressionTest() { input = GetParam().expression; }
};

// Tests

TEST_P(ValidExpressionTest, Tokernizer) {
    auto es = GetParam();
    getNextToken(input);
    string token = getNextToken(input);
    vector<string> tokens = {};
    while ((token = getNextToken()) != "") {
        tokens.push_back(token);
    }
    EXPECT_EQ(es.expected_tokens, tokens);
}

TEST_P(ValidExpressionTest, IntoParseTree) {
    auto es = GetParam();
    std::cout << "The input is: " << input << std::endl;
    shared_ptr<BinTree> tree = parseLine(input);
    std::cout << tree;
    EXPECT_EQ(*(es.expected_tree), *tree);
}

INSTANTIATE_TEST_CASE_P(
    Default, ValidExpressionTest,
    testing::Values(
        expression_state{"     G1   =G2 + ((G3* G4) ^ (G5     -\tG6))",
                         {"G1", "=", "G2", "+", "(", "(", "G3", "*", "G4", ")",
                          "^", "(", "G5", "-", "G6", ")", ")"},
                         getDefaultTree()},
        expression_state{"G1=!G2+((G3*G4)^(G5-G6))",
                         {"G1", "=", "!", "G2", "+", "(", "(", "G3", "*", "G4",
                          ")", "^", "(", "G5", "-", "G6", ")", ")"},
                         getDefaultAndNotTree()},
        expression_state{"G1=!G2", {"G1", "=", "!", "G2"}, getNotTree()},
        expression_state{
            "G1=!(G2+G3)+!G4",
            {"G1", "=", "!", "(", "G2", "+", "G3", ")", "+", "!", "G4"},
            getNotOnBracesTree()},
        expression_state{"G1=load(filename)",
                         {"G1", "=", "load", "(", "filename", ")"},
                         getBasicLoadTree()},
        expression_state{
            "G1 = {x1, x2, x3 | <x1, x2>, <x3, x2>, <x2,x1>,<x3,x1>}",
            {"G1", "=", "{x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}"},
            getBasicLiteralTree()},
        expression_state{
            "G1 = {x3,x4,x5|<x3,x4>,<x3,x5>,<x4,x3>,<x5,x4>}+{x1, x2, x3 | "
            "<x1, x2>, <x3, x2>, <x2,x1>,<x3,x1>}",
            {"G1", "=", "{x3,x4,x5|<x3,x4>,<x3,x5>,<x4,x3>,<x5,x4>}", "+",
             "{x1,x2,x3|<x1,x2>,<x3,"
             "x2>,<x2,x1>,<x3,x1>}"},
            getPlusTwoLiteralsTree()},
        expression_state{
            "G1=load(filename)+((G3*G4)^(G5-G6))",
            {"G1", "=",  "load", "(", "filename", ")",  "+", "(",  "(", "G3",
             "*",  "G4", ")",    "^", "(",        "G5", "-", "G6", ")", ")"},
            getDefaultAndLoadTree()},
        expression_state{
            "G1=load(filename)+((G3*G4)^({x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<"
            "x3,x1>}-G6))",
            {"G1",       "=",
             "load",     "(",
             "filename", ")",
             "+",        "(",
             "(",        "G3",
             "*",        "G4",
             ")",        "^",
             "(",        "{x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}",
             "-",        "G6",
             ")",        ")"},
            getDefaultLoadAndLiteralTree()}));

TEST_P(ValidExpressionTest, IntoParseTreeTwice) {
    auto es = GetParam();
    shared_ptr<BinTree> tree = parseLine(input);
    EXPECT_EQ(*(es.expected_tree), *tree);
    tree = parseLine(input);
    EXPECT_EQ(*(es.expected_tree), *tree);
}
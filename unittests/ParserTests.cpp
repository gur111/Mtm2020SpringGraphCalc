#include <gtest/gtest.h>

#include <vector>
// #include <stdafx.h>

#include <string>

#include "../Parser.h"
using std::string;
using std::vector;
using namespace GraphCalc;

// Fixtures

struct SimpleValidExpressionTest : testing::Test {
    string input;
    shared_ptr<BinTree> expected_tree;
    vector<string> expected_tokens;
    SimpleValidExpressionTest()
        : expected_tree(new BinTree("=")),
          expected_tokens({"G1", "=", "!", "G2", "+", "(", "(", "G3", "*", "G4", ")",
                           "^", "(", "G5", "-", "G6", ")", ")"}) {
        input = "     G1   =!G2 + ((G3* G4) ^ (G5     -\tG6))";
        expected_tree->createLeft("G1");
        expected_tree->createRight("+");
        expected_tree->getRight()->createLeft("G2");
        expected_tree->getRight()->createRight("^");
        expected_tree->getRight()->getRight()->createLeft("*");
        expected_tree->getRight()->getRight()->getLeft()->createLeft("G3");
        expected_tree->getRight()->getRight()->getLeft()->createRight("G4");
        expected_tree->getRight()->getRight()->createRight("-");
        expected_tree->getRight()->getRight()->getRight()->createLeft("G5");
        expected_tree->getRight()->getRight()->getRight()->createRight("G6");
    }
};

// Tests

TEST_F(SimpleValidExpressionTest, Tokernizer) {
    getNextToken(input);
    string token = getNextToken(input);
    vector<string> tokens = {};
    while ((token = getNextToken()) != "") {
        tokens.push_back(token);
    }
    EXPECT_EQ(expected_tokens, tokens);
}

TEST_F(SimpleValidExpressionTest, IntoParseTree) {
    std::cout << "The input is: " << input << std::endl;
    shared_ptr<BinTree> tree = parseLine(input);
    EXPECT_EQ(*expected_tree, *tree);
}

TEST_F(SimpleValidExpressionTest, IntoParseTreeTwice) {
    shared_ptr<BinTree> tree = parseLine(input);
    EXPECT_EQ(*expected_tree, *tree);
    tree = parseLine(input);
    EXPECT_EQ(*expected_tree, *tree);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
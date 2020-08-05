#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "../Graph.h"
using std::endl;
using std::map;
using std::set;
using std::string;
using std::stringstream;
namespace GraphCalc {

struct BasicGraphTest : testing::Test {
    set<string> nodes;
    map<string, set<string>> edges;
    Graph graph;
    string expected_output;

    static map<string, set<string>> get_default_edges() {
        map<string, set<string>> edges;
        edges["x2"] = {"x1", "x4", "x3"};
        edges["x1"] = {"x4", "x3"};
        edges["x4"] = {"x2", "x3"};
        edges["x3"] = {"x1"};
        return edges;
    }

    BasicGraphTest(set<string> nodes = {"x2", "x4", "x3", "x1"},
                   map<string, set<string>> edges = get_default_edges())
        : nodes(nodes), edges(edges), graph(Graph(nodes, edges)) {
        for (auto node : nodes) {
            expected_output += node + "\n";
        }
        expected_output += "$\n";
        for (auto edge : edges) {
            for (auto dnode : edge.second) {
                expected_output += edge.first + +" " + dnode + "\n";
            }
        }
    }
};

// Tests

TEST_F(BasicGraphTest, PrintFormat) {
    stringstream ss;
    ss << graph;
    std::cout << graph;
    EXPECT_EQ(expected_output, ss.str());
}

TEST_F(BasicGraphTest, GraphLiteral) {
    std::cout << Graph("{x1,x2,x3|<x1,x2>}");
}

}  // namespace GraphCalc
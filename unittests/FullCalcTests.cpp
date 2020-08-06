#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "../CalcRunner.h"
using std::cout;
using std::endl;
using std::istringstream;
using std::map;
using std::set;
using std::string;
namespace GraphCalc {

// struct BasicGraphTest : testing::Test {
//     set<string> nodes;
//     map<string, set<string>> edges;
//     Graph graph;
//     string expected_output;

//     BasicGraphTest(set<string> nodes = {"x2", "x4", "x3", "x1"},
//                    map<string, set<string>> edges)
//         : nodes(nodes), edges(edges), graph(Graph(nodes, edges)) {
//         for (auto node : nodes) {
//             expected_output += node + "\n";
//         }
//         expected_output += "$\n";
//         for (auto edge : edges) {
//             for (auto dnode : edge.second) {
//                 expected_output += edge.first + +" " + dnode + "\n";
//             }
//         }
//     }
// };

// Tests

TEST(FullCalcValid, BasicLiteralRead) {
    char str[] = "G1={x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}";
    std::istringstream is(
        string("G1={x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}"));
    GraphCalc calc;
    calc.runCmd("G1={x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}");
    calc.runCmd("G2 = G1");
    calc.runCmd("G3 = G1*G2-G1*G1");
    calc.runCmd("print(G3)");

    // calcRunner(is, cout);
    // EXPECT_EQ(expected_ss.str(), ss.str());
}

}  // namespace GraphCalc
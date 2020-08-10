#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
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
// Fixtures

struct files_state {
    string name;
    string in_filename;
    string out_filename;
};

struct FullCalcInputFileTest : testing::Test,
                               testing::WithParamInterface<files_state> {
    string case_name;
    FullCalcInputFileTest() { case_name = GetParam().name; }
    struct PrintToStringParamName {
        template <class ParamType>
        std::string operator()(
            const testing::TestParamInfo<ParamType> &info) const {
            auto state = static_cast<files_state>(info.param);
            return state.name;
        }
    };
};

// Tests

TEST_P(FullCalcInputFileTest, TestFiles) {
    auto state = GetParam();
    std::ifstream infile;
    std::ifstream outfile;
    infile.open(state.in_filename, std::ios_base::binary);
    infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::ostringstream os;
    std::streambuf *cout_bu;
    cout_bu = std::cout.rdbuf();
    std::cout.rdbuf(os.rdbuf());
    calcRunner(infile, std::cout, false);
    std::cout.rdbuf(cout_bu);
    outfile.open(state.out_filename, std::ios_base::binary);
    outfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    string expected_out((std::istreambuf_iterator<char>(outfile)),
                        std::istreambuf_iterator<char>());
    string output = os.str();
    outfile.close();

    std::regex lines_regex("[^\\n]+");
    auto out_begin =
        std::sregex_iterator(output.begin(), output.end(), lines_regex);
    auto expected_begin = std::sregex_iterator(expected_out.begin(),
                                               expected_out.end(), lines_regex);
    auto lines_end = std::sregex_iterator();
    std::sregex_iterator out = out_begin, exp = expected_begin;
    for (int i = 1; out != lines_end or exp != lines_end; ++out, ++exp, i++) {
        ASSERT_TRUE(out != lines_end);
        ASSERT_TRUE(exp != lines_end);
        std::string out_match_str = (*out).str();
        std::string exp_match_str = (*exp).str();
        cout << "Expected: " << exp_match_str << ". Got: " << out_match_str
             << endl;
        if (out_match_str.find("Error: ") == 0 and
            exp_match_str.find("Error: ") == 0) {
            continue;
        }
        ASSERT_EQ(exp_match_str, out_match_str);
    }
}

INSTANTIATE_TEST_CASE_P(
    Default, FullCalcInputFileTest,
    testing::Values(files_state{"Dani", "../mtm_final_test/dani_input.txt",
                                "../mtm_final_test/dani_output.txt"},
                    files_state{"Gur", "../mtm_final_test/gur_input.txt",
                                "../mtm_final_test/gur_output.txt"},
                    files_state{"Niv", "../mtm_final_test/niv_in.txt",
                                "../mtm_final_test/niv_out.txt"},
                    files_state{"Stav", "../mtm_final_test/stav_in.txt",
                                "../mtm_final_test/stav_out.txt"},
                    files_state{"Shir", "../mtm_final_test/shir_in.txt",
                                "../mtm_final_test/shir_out.txt"},
                    files_state{"Mohammad", "../mtm_final_test/moh_in.txt",
                                "../mtm_final_test/moh_out.txt"},
                    files_state{"2_5_1", "../Tests/2.2&2.5/batch/t1in.txt",
                                "../Tests/2.2&2.5/batch/t1out.txt"},
                    files_state{"2_5_2", "../Tests/2.2&2.5/batch/t2in.txt",
                                "../Tests/2.2&2.5/batch/t2out.txt"},
                    files_state{"2_5_3", "../Tests/2.2&2.5/batch/t3in.txt",
                                "../Tests/2.2&2.5/batch/t3out.txt"}),
    FullCalcInputFileTest::PrintToStringParamName());

TEST(FullCalcValid, BasicLiteralRead) {
    std::istringstream is(
        "G1={x1,x2,x3|<x1,x2>,<x3,x2>,<x2,x1>,<x3,x1>}"
        "\nG2 = !{x1,x3}"
        "\nprint(G2)"
        "\nG3 = {x2, x1}"
        "\nG3 = !G2*G3"
        "\nprint(G3)"
        "\nG4 = G3*G2"
        "\nprint(G4)"
        "\nprint(G1)"
        "\nprint(G2)"
        "\nprint(G3)"
        "\nsave(G1, g4.cg)"
        "\nG3 =load(g4.cg)"
        "\nprint(G1)"
        "\nsave(g2 = {a,[d;],c,b| <a,b>, <[d;],c>, <a,[d;]>},g2.cg)"
        "\ndelete(G3)"
        "\ndelete(G3)");
    string expected =
        "x1\nx3\n$\nx1 x3\nx3 x1"
        "\n[x1;x1]\n[x1;x2]\n[x3;x1]\n[x3;x2]\n$"
        "\n[[x1;x1];x1]\n[[x1;x1];x3]\n[[x1;x2];x1]\n[[x1;x2];x3]\n[[x3;x1];x1]"
        "\n[[x3;x1];x3]\n[[x3;x2];x1]\n[[x3;x2];x3]\n$"
        "\nx1\nx2\nx3\n$\nx1 x2\nx2 x1\nx3 x1\nx3 x2"
        "\nx1\nx3\n$\nx1 x3\nx3 x1"
        "\n[x1;x1]\n[x1;x2]\n[x3;x1]\n[x3;x2]\n$"
        "\nx1\nx2\nx3\n$\nx1 x2\nx2 x1\nx3 x1\nx3 x2"
        "\nError: Missing Declaration. Graph: G3 not declared\n";
    std::ostringstream os;
    std::streambuf *cout_bu;
    cout_bu = std::cout.rdbuf();
    std::cout.rdbuf(os.rdbuf());
    std::streambuf *cin_bu;
    cin_bu = std::cin.rdbuf();
    std::cin.rdbuf(is.rdbuf());
    calcRunner(std::cin, std::cout, false);
    std::cout.rdbuf(cout_bu);
    std::cin.rdbuf(cin_bu);
    ASSERT_EQ(expected, os.str());
}

}  // namespace GraphCalc
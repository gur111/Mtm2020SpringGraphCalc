#include <fstream>
#include <iostream>

#include "CalcRunner.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Interactice mode
        GraphCalc::calcRunner(cin, cout, true);
    } else if (argc == 3) {
        // Auto mode
        ifstream infile;
        ofstream outfile;
        std::streambuf *cout_bu;
        outfile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        infile.open(argv[1], std::ios_base::binary);
        if(not infile.is_open() || infile.bad() || infile.fail()){
            cout << "Failed to open input file for batch mode." << endl;
            return 1;
        }
        infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        outfile.open(argv[2], std::ios_base::binary);
        // Redirect cout to output file
        cout_bu = std::cout.rdbuf();
        std::cout.rdbuf(outfile.rdbuf());

        GraphCalc::calcRunner(infile, outfile, false);
        std::cout.rdbuf(cout_bu);
        infile.close();
        outfile.close();
    } else {
        cerr << "Error: Invalid cmdline arguments" << endl;
    }
}
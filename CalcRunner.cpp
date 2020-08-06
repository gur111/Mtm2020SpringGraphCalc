

#include "GraphCalc.h"

using std::cin;
using std::string;

namespace GraphCalc {
int calcRunner(std::istream &is, std::ostream &os) {
    string line;
    is >> line;
    GraphCalc calc;
    while (line != "") {
        calc.runCmd(line);
        is >> line;
    }
    return 0;
}
}  // namespace GraphCalc
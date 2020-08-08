
#include "Exceptions.h"
#include "GraphCalc.h"

using std::flush;
using std::string;

namespace GraphCalc {
int calcRunner(std::istream &is, std::ostream &os, bool interactive) {
    string line;
    GraphCalc calc;
    while (not is.eof()) {
        if (interactive) {
            os << "Gcalc> " << flush;
        }
        std::getline(is, line);
        try {
            if (not calc.runCmd(line)) {
                break;
            }
        } catch (const GraphCalcError &exp) {
            os << exp.what() << std::endl;
        } catch (const std::exception &exp) {
            os << "Error: Unknown error. Got: " << exp.what() << std::endl;
        }
    }
    return 0;
}  // namespace GraphCalc
}  // namespace GraphCalc
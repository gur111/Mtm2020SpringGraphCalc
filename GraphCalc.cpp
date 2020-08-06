#include "GraphCalc.h"

#include <iostream>

#include "Parser.h"
#include "TreeRunner.h"

using std::shared_ptr;
using std::string;

namespace GraphCalc {
void GraphCalc::runCmd(string command) {
    shared_ptr<BinTree> tree = parseLine(command);
    std::cout << tree << std::endl;
    executeTree(storage, tree);
}
}  // namespace GraphCalc

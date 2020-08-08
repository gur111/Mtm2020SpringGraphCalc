#include "GraphCalc.h"

#include <iostream>

#include "Parser.h"
#include "TreeRunner.h"

using std::shared_ptr;
using std::string;

namespace GraphCalc {
bool GraphCalc::runCmd(string command) {
    shared_ptr<BinTree> tree = parseLine(command);
    if (tree->isLeaf() and tree->get() == "quit") {
        return false;
    }
    executeTree(storage, tree);
    return true;
}
}  // namespace GraphCalc

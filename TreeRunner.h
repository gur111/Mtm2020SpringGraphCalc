#ifndef TREE_RUNNER_H
#define TREE_RUNNER_H

#include <memory>

#include "BinTree.h"
#include "Graph.h"
#include "StorageManager.h"

namespace GraphCalc {
std::shared_ptr<Graph> executeTree(StorageManager &storage,
                                   std::shared_ptr<BinTree> tree);

}

#endif
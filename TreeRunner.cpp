

#include "TreeRunner.h"

#include "Constants.h"

using std::shared_ptr;
using std::string;

namespace GraphCalc {
shared_ptr<Graph> executeTree(StorageManager storage,
                              shared_ptr<BinTree> tree) {
    if (tree->get() == "load" or
        UNARY_OPERATORS.find(tree->get()) != UNARY_OPERATORS.end()) {
        if (tree->getRight() != nullptr) {
            // TODO: Throw exception, unary operator has right argument
        }
        if (tree->getLeft() == nullptr) {
            // TODO: Throw exception, unary operator missing operand
        }
        shared_ptr<Graph> graph = executeTree(storage, tree->getLeft());
        if (tree->get() == "!") {
            return shared_ptr<Graph>(new Graph(!*graph));
        } else if (tree->get() == "load") {
            return graph;  // TODO: Handle load
        }
    } else if (BIN_OPERATORS.find(tree->get()) != BIN_OPERATORS.end()) {
        if (tree->getLeft() == nullptr or tree->getRight() == nullptr) {
            // TODO: Throw exception, binary operator missing operand
        }

        shared_ptr<Graph> graphR = executeTree(storage, tree->getRight());
        if (tree->get() == "=") {
            // TODO: Validate left name
            return storage.set(tree->getLeft()->get(), graphR);
        }
        shared_ptr<Graph> graphL = executeTree(storage, tree->getLeft());

        if (tree->get() == "+") {
            return shared_ptr<Graph>(new Graph((*graphL) + (*graphR)));
        } else if (tree->get() == "-") {
            return shared_ptr<Graph>(new Graph((*graphL) - (*graphR)));
        } else if (tree->get() == "*") {
            return shared_ptr<Graph>(new Graph((*graphL) * (*graphR)));
        } else if (tree->get() == "^") {
            return shared_ptr<Graph>(new Graph((*graphL) ^ (*graphR)));
        }
    } else /* TODO:  if (is valid name/graph literal) */ {
        if (not tree->isLeaf()) {
            // TODO: Throw exception, varname/graph literal has a child
            // TODO: Handle getting graph literals as well
            return storage.get(tree->get());
        }
    }
}
}  // namespace GraphCalc
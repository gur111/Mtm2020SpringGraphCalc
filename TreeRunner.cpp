

#include "TreeRunner.h"

#include "Constants.h"
#include "Utils.h"

using std::shared_ptr;
using std::string;

namespace GraphCalc {
shared_ptr<Graph> executeTree(StorageManager &storage,
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
        if (graph == nullptr) {
            // TODO: Throw exception. Sub expression has a null value
            return nullptr;
        }
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
        if (graphR == nullptr) {
            // TODO: Throw exception. Sub expression has a null value
            return nullptr;
        }
        if (tree->get() == "=") {
            // TODO: Validate left name
            return storage.set(tree->getLeft()->get(), graphR);
        }
        shared_ptr<Graph> graphL = executeTree(storage, tree->getLeft());
        if (graphL == nullptr) {
            // TODO: Throw exception. Sub expression has a null value
            return nullptr;
        }

        if (tree->get() == "+") {
            return shared_ptr<Graph>(new Graph((*graphL) + (*graphR)));
        } else if (tree->get() == "-") {
            return shared_ptr<Graph>(new Graph((*graphL) - (*graphR)));
        } else if (tree->get() == "*") {
            return shared_ptr<Graph>(new Graph((*graphL) * (*graphR)));
        } else if (tree->get() == "^") {
            return shared_ptr<Graph>(new Graph((*graphL) ^ (*graphR)));
        }
    } else if (tree->get() == "print") {
        // No need to verify there's no right node because of the way parser
        // builds it, it can never happen
        std::cout << *executeTree(storage, tree->getLeft());
        return nullptr;
    } else if (tree->get() == "delete") {
        // No need to verify format. If the var is invalid name, it won't exist
        // so an error will be thrown either way
        storage.remove(tree->getRight()->get());
        return nullptr;
    } else if (tree->get() == "save") {
        // TODO: Verify right is leaf and filename is valid
        executeTree(storage, tree->getLeft())
            ->saveToFile(tree->getRight()->get());
        return nullptr;
    } else if (tree->get() == "reset") {
        // Again, no need to verify it's a leaf
        storage.reset();
        return nullptr;
    } else if (tree->get() == "who") {
        storage.who();
        return nullptr;
    } else /* TODO:  if (is valid name/graph literal) */ {
        if (not tree->isLeaf()) {
            // TODO: Throw exception, varname/graph literal has a child
            // TODO: Handle getting graph literals as well
        }
        string literal = extractGraphLiteralToken(tree->get());
        if (literal != "") {
            return shared_ptr<Graph>(new Graph(literal));
        } else {
            // TODO: Validate name
            return storage.get(tree->get());
        }
    }
}
}  // namespace GraphCalc


#include "TreeRunner.h"

#include "Constants.h"
#include "Exceptions.h"
#include "Utils.h"

using std::shared_ptr;
using std::string;

namespace GraphCalc {
shared_ptr<Graph> executeTree(StorageManager &storage,
                              shared_ptr<BinTree> tree) {
    if (tree->get() == "") {
        return nullptr;
    } else if (tree->get() == "load" or
               UNARY_OPERATORS.find(tree->get()) != UNARY_OPERATORS.end()) {
        if (tree->getRight() != nullptr) {
            throw InvalidFormat("Unary operator has right argument.");
        }
        if (tree->getLeft() == nullptr) {
            throw InvalidFormat("Unary operator missing operand.");
        }
        shared_ptr<Graph> graph;
        if (tree->get() == "load") {
            graph = Graph::loadFromFile(tree->getLeft()->get());
        } else if (tree->get() == "!") {
            graph = executeTree(storage, tree->getLeft());
        }
        if (graph == nullptr) {
            throw InvalidFormat("Sub expressions has a null value.");
        }
        if (tree->get() == "!") {
            return shared_ptr<Graph>(new Graph(!*graph));
        } else if (tree->get() == "load") {
            return graph;
        } else {
            throw Unknown("This should've never happened.");
        }
    } else if (BIN_OPERATORS.find(tree->get()) != BIN_OPERATORS.end()) {
        if (tree->getLeft() == nullptr or tree->getRight() == nullptr) {
            throw InvalidFormat("Binary operator \"" + tree->get() +
                                "\" missing operand.");
        }
        shared_ptr<Graph> graphR = executeTree(storage, tree->getRight());
        if (graphR == nullptr) {
            throw InvalidFormat("Sub expressions has a null value.");
        }
        if (tree->get() == "=") {
            auto left = tree->getLeft();
            if (not left->isLeaf() || not isValidGraphName(left->get())) {
                throw InvalidFormat("Invalid expression left to a \"=\"");
            }
            return storage.set(tree->getLeft()->get(), graphR);
        }
        shared_ptr<Graph> graphL = executeTree(storage, tree->getLeft());
        if (graphL == nullptr) {
            throw InvalidFormat("Sub expressions has a null value.");
        }

        if (tree->get() == "+") {
            return shared_ptr<Graph>(new Graph((*graphL) + (*graphR)));
        } else if (tree->get() == "-") {
            return shared_ptr<Graph>(new Graph((*graphL) - (*graphR)));
        } else if (tree->get() == "*") {
            return shared_ptr<Graph>(new Graph((*graphL) * (*graphR)));
        } else if (tree->get() == "^") {
            return shared_ptr<Graph>(new Graph((*graphL) ^ (*graphR)));
        } else {
            throw Unknown("Unknown operator: " + tree->get());
        }
    } else if (tree->get() == "print") {
        // No need to verify there's no right node because of the way parser
        // builds it, it can never happen
        std::cout << *executeTree(storage, tree->getLeft());
        return nullptr;
    } else if (tree->get() == "delete") {
        // No need to verify format. If the var is invalid name, it won't exist
        // so an error will be thrown either way
        storage.remove(tree->getLeft()->get());
        return nullptr;
    } else if (tree->get() == "save") {
        // TODO: Verify right is leaf and filename is valid
        if (not tree->getRight()->isLeaf()) {
            throw InvalidFormat(
                "Invalid second operand for \"save\" function.");
        }
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
    } else {
        if (not tree->isLeaf()) {
            throw InvalidFormat("Var-name/graph-literal has a child.");
        }
        string literal = extractGraphLiteralToken(tree->get());
        if (literal != "") {
            return shared_ptr<Graph>(new Graph(literal));
        } else {
            if (not isValidGraphName(tree->get())) {
                throw InvalidFormat("Graph name " + tree->get() +
                                    " is invalid.");
            }
            return storage.get(tree->get());
        }
    }
}
}  // namespace GraphCalc
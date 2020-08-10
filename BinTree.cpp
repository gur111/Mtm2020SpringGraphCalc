#include "BinTree.h"

#include "Exceptions.h"

#define TREE_PRINT_SPREAD 10
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;
using std::string;

namespace GraphCalc {
BinTree::BinTree(string value) : left(nullptr), right(nullptr), value(value) {}

BinTree::~BinTree() {
    removeLeft();
    removeRight();
}

void BinTree::set(string value) { this->value = value; }

string BinTree::get() const { return value; }

shared_ptr<BinTree> BinTree::getLeft() { return left; }

shared_ptr<BinTree> BinTree::getRight() { return right; }

void BinTree::setLeft(shared_ptr<BinTree> sub_tree) { this->left = sub_tree; }

void BinTree::setRight(shared_ptr<BinTree> sub_tree) { this->right = sub_tree; }

shared_ptr<BinTree> BinTree::createLeft(string value) {
    if (left) {
        throw MultipleDeclarations("Can't create \"" + value +
                                   "\" Left child in tree (\"" + left->get() +
                                   "\") already exist.");
    }
    return left = shared_ptr<BinTree>(new BinTree(value));
}
shared_ptr<BinTree> BinTree::createRight(string value) {
    if (right) {
        throw MultipleDeclarations("Right child in tree already exist.");
    }
    return right = shared_ptr<BinTree>(new BinTree(value));
}

void BinTree::removeLeft() { left = nullptr; }
void BinTree::removeRight() { right = nullptr; }
bool BinTree::operator==(const BinTree &other) const {
    if (value != other.value) {
        return false;
    }
    if (left and other.left) {
        if (*left != *(other.left)) {
            return false;
        }
    } else if (left != nullptr or other.left != nullptr) {
        return false;
    }
    if (right and other.right) {
        if (*right != *(other.right)) {
            return false;
        }
    } else if (right != nullptr or other.right != nullptr) {
        return false;
    }
    return true;
}

bool BinTree::operator!=(const BinTree &other) const {
    return not(*this == other);
}
bool BinTree::isLeaf() const { return (left == nullptr and right == nullptr); }

void GraphCalc::BinTree::cleanup() {
    if (left) {
        left->cleanup();
    }
    if (right) {
        right->cleanup();
    }

    if (value == "") {
        shared_ptr<BinTree> side = nullptr;
        if (left == nullptr and right != nullptr) {
            side = right;
        } else if (left != nullptr and right == nullptr) {
            side = left;
        } else {
            if (left == nullptr and right == nullptr) {
                throw Unknown("Likely empty braces.");
            } else {
                throw SyntaxError("Expected operator between operands.");
            }
            return;
        }
        set(side->value);
        left = side->left;
        right = side->right;
    }
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void BinTree::print2DUtil(ostream &os, const shared_ptr<BinTree> root,
                          int space) {
    // Base case
    if (root == nullptr) return;

    // Increase distance between levels
    space += TREE_PRINT_SPREAD;

    // Process right child first
    print2DUtil(os, root->right, space);

    // Print current node after space
    // count
    cout << endl;
    for (int i = TREE_PRINT_SPREAD; i < space; i++) cout << " ";
    if (root->value == "") {
        cout << "NV" << endl;
    }
    cout << root->value << endl;

    // Process left child
    print2DUtil(os, root->left, space);
}

ostream &operator<<(ostream &os, const shared_ptr<BinTree> tree) {
    BinTree::print2DUtil(os, tree, 0);
    os << "\n" << endl;
    return os;
}

}  // namespace GraphCalc

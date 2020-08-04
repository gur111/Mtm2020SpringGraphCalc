#include "BinTree.h"

#include <iostream>

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
        // TODO: Throw `already exists` exception
    }
    return left = shared_ptr<BinTree>(new BinTree(value));
}
shared_ptr<BinTree> BinTree::createRight(string value) {
    if (right) {
        // TODO: Throw `already exists` exception
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
                std::cout << "ERROR: leaf contains an empty value" << std::endl;
            } else {
                std::cout << "ERROR: Expected operator between operands"
                          << std::endl;
            }
            return;
        }
        set(side->value);
        left = side->left;
        right = side->right;
    }
}

}  // namespace GraphCalc

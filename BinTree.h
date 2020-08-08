#ifndef BINTREE_H
#define BINTREE_H
#include <iostream>
#include <memory>
#include <string>

namespace GraphCalc {

class BinTree {
    std::shared_ptr<BinTree> left;
    std::shared_ptr<BinTree> right;
    std::string value;

   public:
    /**
     * Creates an BinTree with a single node containing `value`
     */
    BinTree(std::string value);
    /**
     * Deletes the tree
     */
    ~BinTree();
    /**
     * Sets the value
     */
    void set(std::string value);
    /**
     * Gets the value
     */
    std::string get() const;
    /**
     * Gets the left element
     */
    std::shared_ptr<BinTree> getLeft();
    /**
     * Gets the right element
     */
    std::shared_ptr<BinTree> getRight();
    /**
     * Sets the left element
     */
    void setLeft(std::shared_ptr<BinTree> sub_tree);
    /**
     * Sets the right element
     */
    void setRight(std::shared_ptr<BinTree> sub_tree);
    /**
     * Creates a left element with value.
     * If one already existsm throw exception
     */
    std::shared_ptr<BinTree> createLeft(std::string value);
    /**
     * Creates a right element with value.
     * If one already existsm throw exception
     */
    std::shared_ptr<BinTree> createRight(std::string value);
    /**
     * Remove the left element. Putting nullptr instead
     */
    void removeLeft();
    /**
     * Remove the right element. Putting nullptr instead
     */
    void removeRight();
    /**
     * Streams string output to the given ostream
     */
    static void print2DUtil(std::ostream &os,
                            const std::shared_ptr<BinTree> root, int space);
    /**
     * Compare two trees to make sure they contain the same values
     */
    bool operator==(const BinTree &other) const;
    bool operator!=(const BinTree &other) const;
    bool isLeaf() const;
    void cleanup();
};
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<BinTree> tree);
}  // namespace GraphCalc

#endif
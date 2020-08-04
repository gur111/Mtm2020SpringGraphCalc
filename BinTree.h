#ifndef BINTREE_H
#define BINTREE_H
#include <memory>
#include <string>

namespace GraphCalc {
using std::shared_ptr;
using std::string;

class BinTree {
    string value;
    shared_ptr<BinTree> left;
    shared_ptr<BinTree> right;

   public:
    /**
     * Creates an BinTree with a single node containing `value`
     */
    BinTree(string value);
    /**
     * Deletes the tree
     */
    ~BinTree();
    /**
     * Sets the value
     */
    void set(string value);
    /**
     * Gets the value
     */
    string get() const;
    /**
     * Gets the left element
     */
    shared_ptr<BinTree> getLeft();
    /**
     * Gets the right element
     */
    shared_ptr<BinTree> getRight();
    /**
     * Sets the left element
     */
    void setLeft(shared_ptr<BinTree> sub_tree);
    /**
     * Sets the right element
     */
    void setRight(shared_ptr<BinTree> sub_tree);
    /**
     * Creates a left element with value.
     * If one already existsm throw exception
     */
    shared_ptr<BinTree> createLeft(string value);
    /**
     * Creates a right element with value.
     * If one already existsm throw exception
     */
    shared_ptr<BinTree> createRight(string value);
    /**
     * Remove the left element. Putting nullptr instead
     */
    void removeLeft();
    /**
     * Remove the right element. Putting nullptr instead
     */
    void removeRight();
    /**
     * Compare two trees to make sure they contain the same values
     */
    bool operator==(const BinTree &other) const;
    bool operator!=(const BinTree &other) const;
    void cleanup();
};
}  // namespace GraphCalc

#endif
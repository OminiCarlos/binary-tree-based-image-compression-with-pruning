/**
 *  @file imgtree.cpp
 *  @description instructor-provided implementations for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  THIS FILE WILL NOT BE SUBMITTED TO PRAIRIELEARN
 */

#include "imgtree.h"

/**
 *  Copy constructor creates a new tree that is structurally the same as the input tree and
 *  contains the same image data.
 *  @param other - the ImgTree to be copied
 */
ImgTree::ImgTree(const ImgTree& other) {
    // This implementation has already been completed for you
    Copy(other);
}

/**
 *  Overloaded assignment operator allows statements such as mytree_a = some_other_tree;
 *  Releases any existing memory and reconstructs this tree as a copy of other.
 *  @param rhs - the right hand side of the assignment statement
 */
ImgTree& ImgTree::operator=(const ImgTree& rhs) {
    // This implementation has already been completed for you
    if (this != &rhs) { // this and rhs are physically different trees in memory
        Clear(); // release any existing heap memory for this tree
        Copy(rhs);  // rebuild this tree as a copy of rhs
    }
    return *this;
}

/**
 *  Destructor releases any heap memory associated with this tree.
 */
ImgTree::~ImgTree() {
    // This implementation has already been completed for you
    Clear();
}
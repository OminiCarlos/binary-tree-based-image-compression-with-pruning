/**
 *  @file imgtree.h
 *  @description definition of a binary tree class storing image data for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  THIS FILE WILL NOT BE SUBMITTED TO PRAIRIELEARN
 */

#ifndef _IMGTREE_H_
#define _IMGTREE_H_

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"

#include <utility>

using namespace std;
using namespace cs221util;

/**
 *  Node definition for the tree class.
 *  This is made public for testing, but in practice should be private.
 *  The end-user does not need to know about the implementation details
 *  of the tree, and only needs to know what functions can be called.
 */
class ImgTreeNode {
    public:

        unsigned int upper; // image coordinates of this node's covered region
        unsigned int left;
        unsigned int lower;
        unsigned int right;
        RGBAPixel avg;

        ImgTreeNode* A; // ptr to left or upper subtree
        ImgTreeNode* B; // ptr to right or lower subtree

        /**
        *  Constructs an ImgTreeNode with null children
        */
        ImgTreeNode(unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt, RGBAPixel average) {
            upper = upr;
            left = lft;
            lower = lwr;
            right = rt;
            avg = average;
            A = nullptr;
            B = nullptr;
        }
};

class ImgTree {
    private:
        /**
         *  Structure which holds information about potential image split candidates.
         *  A collection of these will be used during tree construction to determine the best place to split.
         */
        typedef struct SplitInfo {
            double sumsqscore; // combined sum square difference of the two image parts being split
            unsigned int coordinate; // coordinate (either x or y, depending on current split orientation) 
                                     // which produces the recorded sum squared difference score above.
        } SplitInfo;

        ImgTreeNode* root; // initial point of access into the tree

        unsigned int imgwidth;  // width of the image represented by the tree, in pixels
        unsigned int imgheight; // height of the image represented by the tree, in pixels

        /**
         *  Releases all heap memory associated with this tree, restoring it to an "empty tree" state.
         *  Will be useful to define a recursive helper function for this.
         */
        void Clear();

        /**
         *  Copies the supplied parameter tree into this tree. Does not free any memory.
         *  Called by the copy constructor and operator=.
         *  Will be useful to define a recursive helper function for this.
         *  HINT: the helper should allocate a new node, and return a pointer to the allocated node.
         *        See the documention for BuildNode - this should work similarly.
         */
        void Copy(const ImgTree& other);

        /**
         *  Recursive helper function for initial construction of the tree. Constructs a single
         *  node according to supplied Stats and the requirements specified by the constructor
         *  documentation, and returns a pointer to the completed node.
         *  @param s - populated Stats object for computing this node's attributes
         *  @param upr - y-coordinate of the upper edge of the node's rectangular region
         *  @param lft - x-coordinate of the left side of the node's rectangular region
         *  @param lwr - y-coordinate of the lower edge of the node's rectangular region
         *  @param rt - x-coordinate of the right side of the node's rectangular region
         *  @return - pointer to a (completed) newly-allocated node for the specified parameters.
         */
        ImgTreeNode* BuildNode(Stats& s, unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt);

    public:

        /**
         *  Default constructor creates an empty tree
         */
        ImgTree();

        /**
         *  Parameterized constructor creates a tree from an input image.
         *  Every leaf in the tree corresponds to a single pixel in the PNG.
         *  Every non-leaf node corresponds to a rectangle of pixels defined
         *  by upper, left, lower, and right image coordinates. A node will
         *  also store the average color over its defined rectangle. Note that
         *  this average MUST be freshly computed from the individual pixels in
         *  the image and NOT by computing a weighted average from the colors
         *  of its direct children, as this would lead to an accumulation of
         *  rounding errors due to the integer nature of RGB color channels.
         *  
         *  A non-leaf node's children correspond to a partition of the node's
         *  rectangle into two smaller rectangles, either by a horizontal line
         *  creating an upper rectangle and a lower rectangle, or a vertical line
         *  creating a left rectangle and a right rectangle.
         * 
         *  The split is determined as follows:
         *  1. If the current node's rectangle width is the same or larger than its height,
         *     then a vertical line will divide the rectangle into left and right rectangles.
         *     If the current node's rectangle width is smaller than its height,
         *     then a horizontal line will divide the rectangle into upper and lower rectangles.
         *  2. The coordinate of the dividing line is chosen such that combined sum squared
         *     deviations from the mean color in the left/upper and right/lower rectangles is minimal
         *     among all other potential dividing lines of the same orientation.
         *     e.g. for a region (0,0) to (3,2), the candidates are the vertical lines dividing the region into:
         *     - (0,0) to (0,2) and (1,0) to (3,2)
         *     - (0,0) to (1,2) and (2,0) to (3,2)
         *     - (0,0) to (2,2) and (3,2) to (3,2)
         *     The line which produces the minimal combined sum squared scores on the left
         *     and right will be used for the split.
         *  3. In the unlikely event that multiple candidates produce the same score, the one which
         *     most evenly splits the rectangular area will be chosen.
         *  4. In the even more unlikely even that two candidates produce the same score and produce
         *     the same minimal area difference, the one with the smaller coordinate will be chosen.
         */
        ImgTree(const PNG& img);

        /**
         *  Copy constructor creates a new tree that is structurally the same as the input tree and
         *  contains the same image data.
         *  @param other - the ImgTree to be copied
         */
        ImgTree(const ImgTree& other);

        /**
         *  Overloaded assignment operator allows statements such as mytree_a = some_other_tree;
         *  Releases any existing memory and reconstructs this tree as a copy of other.
         *  @param rhs - the right hand side of the assignment statement
         */
        ImgTree& operator=(const ImgTree& rhs);

        /**
         *  Destructor releases any heap memory associated with this tree.
         */
        ~ImgTree();

        /**
         *  Produces a PNG of appropriate dimensions and paints every leaf node's rectangle
         *  into the appropriate area of the PNG.
         *  May be called on pruned trees.
         *  @param scaling factor for how large to render the image. Assume this is >= 1.
         *  @return fully-colored PNG, painted from the tree's leaf node data
         */
        PNG Render(unsigned int scale) const;

        /**
         *  Rearranges a tree's internal pointers and node content so that its image data
         *  appears flipped horizontally when rendered.
         *  Beware that the tree may or may not have been pruned!
         *  Will be useful to define a recursive helper function for this.
         */
        void FlipHorizontal();

        /**
         *  Trims subtrees as high as possible in the tree.
         *  A subtree is pruned (all decendants deallocated and child links set to null) 
         *  if at least pct (out of 100) of its leaves are within tol of the average
         *  color in the subtree's root.
         *  Assume that this will only be called on trees which have not previously been pruned.
         *  Will be useful to define AT LEAST one recursive helper function for this.
         *  @pre pct is a valid value between 0 and 100
         *  @param pct percentage (out of 100) of leaf node descendants must be within the tolerance threshold
         *             of color difference in order to be pruned
         *  @param tol threshold color difference to qualify for pruning
         */
        void Prune(double pct, double tol);

        /**
         *  Counts the number of leaf nodes in the tree.
         *  Will be useful to define a recursive helper function for this.
         */
        unsigned int CountLeaves() const;

    private:
        // In the file included below, write your definitions for any private functions you find useful.
        #include "imgtree-private.h"
};

#endif
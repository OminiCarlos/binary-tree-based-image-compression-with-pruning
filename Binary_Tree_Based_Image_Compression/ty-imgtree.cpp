/**
 *  @file imgtree.cpp
 *  @description implementation of a binary tree class storing image data for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  SUBMIT THIS FILE TO PRAIRIELEARN
 */

#include "imgtree.h"
#include <limits>

// not necessary to include imgtree-private.h since it is already included in imgtree.h

/**
 *  Default constructor creates an empty tree
 */
ImgTree::ImgTree() {
    // complete your implementation below
    root = nullptr;
    imgwidth = 0;
    imgheight = 0;

}

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
ImgTree::ImgTree(const PNG& img) {
    // complete your implementation below

    imgheight = img.height();
    imgwidth = img.width();

    Stats stats(img);
    root = BuildNode(stats, 0, 0, imgheight-1, imgwidth-1);

}

/**
 *  Releases all heap memory associated with this tree, restoring it to an "empty tree" state.
 *  Will be useful to define a recursive helper function for this.
 */
void ImgTree::Clear() {
    // complete your implementation below
    ClearHelper(root);
    root = nullptr;

}

/**
 *  Copies the supplied parameter tree into this tree. Does not free any memory.
 *  Called by the copy constructor and operator=.
 *  Will be useful to define a recursive helper function for this.
 *  HINT: the helper should allocate a new node, and return a pointer to the allocated node.
 *        See the documention for BuildNode - this should work similarly.
 */
void ImgTree::Copy(const ImgTree& other) {
    // complete your implementation below
    imgwidth = other.imgwidth;
    imgheight = other.imgheight;

    root = CopyNode(other.root);

}

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
ImgTreeNode* ImgTree::BuildNode(Stats& s, unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt) {
    // complete your implementation below

    RGBAPixel average = s.GetAvg(upr, lft, lwr, rt);
    ImgTreeNode* node = new ImgTreeNode(upr, lft, lwr, rt, average);

    // base case: there is only one pixel
    // that is, upr == lwr && lft == rt 
    if (upr == lwr && lft == rt) {
        
        return node;
    }

    SplitInfo bestSplit = FindBestSplit(s, upr, lft, lwr, rt);

    if (rt - lft >= lwr - upr) { 
        // width >= height ->  vertical split
        node->A = BuildNode(s, upr, lft, lwr, bestSplit.coordinate);
        node->B = BuildNode(s, upr, bestSplit.coordinate + 1, lwr, rt);
    } else { 
        // horizontal
        node->A = BuildNode(s, upr, lft, bestSplit.coordinate, rt);
        node->B = BuildNode(s, bestSplit.coordinate + 1, lft, lwr, rt);
    }

    return node; // REPLACE THIS STUB
}

/**
 *  Produces a PNG of appropriate dimensions and paints every leaf node's rectangle
 *  into the appropriate area of the PNG.
 *  May be called on pruned trees.
 *  @param scaling factor for how large to render the image. Assume this is >= 1.
 *  @return fully-colored PNG, painted from the tree's leaf node data
 */
PNG ImgTree::Render(unsigned int scale) const {
    // complete your implementation below

    PNG result(imgwidth * scale, imgheight * scale);

    RenderNode(root, result, scale);

    return result; // REPLACE THIS STUB
}

/**
 *  Rearranges a tree's internal pointers and node content so that its image data
 *  appears flipped horizontally when rendered.
 *  Beware that the tree may or may not have been pruned!
 *  Will be useful to define a recursive helper function for this.
 */
void ImgTree::FlipHorizontal() {
    // complete your implementation below

    FlipHorizontalHelper(root);

}

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
void ImgTree::Prune(double pct, double tol) {
    // complete your implementation below

    PruneHelper(pct, tol, root);

}

/**
 *  Counts the number of leaf nodes in the tree.
 *  Will be useful to define a recursive helper function for this.
 */
unsigned int ImgTree::CountLeaves() const {
    // complete your implementation below

    return CountNodeLeaves(root); // REPLACE THIS STUB
}

/**
 *  ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS BELOW
 */

    
ImgTree::SplitInfo ImgTree::FindBestSplit(Stats& s, unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt){

    SplitInfo bestSplit;
    bestSplit.sumsqscore = std::numeric_limits<double>::max();
    if (rt - lft >= lwr -  upr) { 
        //split vertically
        for (unsigned int i=lft; i < rt; i++) {
            double sumSqDev = s.GetSumSqDev(upr, lft, lwr, i) + s.GetSumSqDev(upr, i + 1, lwr, rt);
            if (sumSqDev < bestSplit.sumsqscore) {
                bestSplit.sumsqscore = sumSqDev;
                bestSplit.coordinate = i;
            } else if (sumSqDev == bestSplit.sumsqscore) {
                int currLeft = (int)(i - lft + 1);
                int currRight = (int)(rt - i);
                int left = (int)(bestSplit.coordinate - lft + 1);
                int right = (int)(rt - bestSplit.coordinate);
                if (abs(currLeft - currRight) < abs(left - right)) {
                    bestSplit.coordinate = i;
                } else if (abs(currLeft - currRight) == abs(left - right)) {
                    if (i < bestSplit.coordinate) {
                        // this case is not gonna happen
                        bestSplit.coordinate = i;
                    }
                }
            }
        }
    } else {
        // split horizontally
        
        for (unsigned int i=upr; i < lwr; i++) {
            double sumSqDev = s.GetSumSqDev(upr, lft, i, rt) + s.GetSumSqDev(i+1, lft, lwr, rt);
            if (sumSqDev < bestSplit.sumsqscore) {
                bestSplit.sumsqscore = sumSqDev;
                bestSplit.coordinate = i;
            } else if (sumSqDev == bestSplit.sumsqscore) {
                int currUpper = (int)(i - upr + 1);
                int currLower = (int)(lwr - i);
                int upper = (int)(bestSplit.coordinate - upr + 1);
                int lower = (int)(lwr - bestSplit.coordinate);
                if (abs(currUpper - currLower) < abs(upper - lower)) {
                    bestSplit.coordinate = i;
                } else if (abs(currUpper - currLower) < abs(upper - lower))  {
                    if (i < bestSplit.coordinate) {
                        // this case is not gonna happen
                        bestSplit.coordinate = i;
                    }
                }
            }
        }
    }
    return bestSplit;
}


/**
 * Helper function for Render
 */
void ImgTree::RenderNode(ImgTreeNode* node, PNG& img, unsigned int scale) const {
    if (node == nullptr) {
        return;
    }

    // leaf node, print
    if (node -> A == nullptr && node -> B == nullptr) {
    
        for (unsigned int y = node->upper * scale; y < (node->lower + 1) * scale; y++) {
            for (unsigned int x = node->left * scale; x < (node->right + 1) * scale; x++) {
                *img.getPixel(x, y) = node->avg;
            }
        }
        
    } else {
        RenderNode(node->A, img, scale);
        RenderNode(node->B, img, scale);
    }
}


/**
 * Helper function for Copy()
 */
ImgTreeNode* ImgTree::CopyNode(ImgTreeNode* node) {

    ImgTreeNode* target = nullptr;

    if (node == nullptr) {
        return target;
    }

    // when node is not nullptr
    target = new ImgTreeNode(node->upper, node->left, node->lower, node->right, node->avg);
    
    // if node's A and B are not nullptr, copy A and B to target
    if (node->A != nullptr && node->B != nullptr) {
        target->A = CopyNode(node->A);
        target->B = CopyNode(node->B);
    }

    return target;
}





/**
 * Helper function for FlipHorizontal()
 */
void ImgTree::FlipHorizontalHelper(ImgTreeNode* node) {

    if (node == nullptr) {
        return;
    }

    // Swap the left and right boundaries for the current node
    unsigned int tempLeft = node->left;
    node->left = imgwidth - 1 - node->right;
    node->right = imgwidth - 1 - tempLeft;

    // if the current node has children, process its children
    if (node->A != nullptr && node->B != nullptr) {
        FlipHorizontalHelper(node->A);
        FlipHorizontalHelper(node->B);
        // if the current node is "wide", swap its children
        if (node->right - node->left >= node->lower - node->upper) {
            ImgTreeNode* temp = node->A;
            node->A = node->B;
            node->B = temp;
        }
    }
}

/**
 * Helper function for CountLeaves()
 */
unsigned int ImgTree::CountNodeLeaves(ImgTreeNode* node) const {

    if (node == nullptr) {
        return 0;
    }

    // when the node it self is a leaf
    if (node->A == nullptr && node->B == nullptr) {
        return 1;
    }

    // when the node itself is not a leaf
    return CountNodeLeaves(node->A) + CountNodeLeaves(node->B);
}

void ImgTree::ClearHelper(ImgTreeNode* node) {
    if (node == nullptr) {
        return;
    }

    ClearHelper(node->A);
    ClearHelper(node->B);

    delete node;
}


/**
 * Helper function for Prune()
 */
void ImgTree::PruneHelper(double pct, double tol, ImgTreeNode* node) {
    // if percentage >= pct, prune by clearing node->A and node->B
    // else, call PruneHelper on node->A and node->B

    if (node == nullptr) {
        return;
    }

    unsigned int count = CountLeavesWithinTolerance(tol, node->avg, node);
    if ((double)count / CountNodeLeaves(node) >= pct/100 ) {
        ClearHelper(node->A);
        ClearHelper(node->B);
        node->A = nullptr;
        node->B = nullptr;
    } else {
        PruneHelper(pct, tol, node->A);
        PruneHelper(pct, tol, node->B);
    }
}

unsigned int ImgTree::CountLeavesWithinTolerance(double tol, RGBAPixel& avg, ImgTreeNode* node) {

    if (node == nullptr) {
        return 0;
    }

    // current node is not a leaf
    if (node->A != nullptr && node->B != nullptr) {
        return CountLeavesWithinTolerance(tol, avg, node->A) + CountLeavesWithinTolerance(tol, avg, node->B);
    }

    // current node is a leaf, process it 
    double colorDiff = CalcColorDiff(avg, node->avg);
    if (colorDiff <= tol) {
        return 1;
    } else {
        return 0;
    }
}


double ImgTree::CalcColorDiff(const RGBAPixel& a, const RGBAPixel& b) const {
    // return std::sqrt(std::pow(a.r - b.r, 2) + std::pow(a.g - b.g, 2) + std::pow(a.b - b.b, 2) + std::pow(a.a - b.a, 2));
    return a.dist(b);
}
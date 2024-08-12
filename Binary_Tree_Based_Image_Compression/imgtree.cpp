/**
 *  @file imgtree.cpp
 *  @description implementation of a binary tree class storing image data for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  SUBMIT THIS FILE TO PRAIRIELEARN
 */

#include "imgtree.h"
// not necessary to include imgtree-private.h since it is already included in imgtree.h

/**
 *  Default constructor creates an empty tree
 */
ImgTree::ImgTree()
{
    root = nullptr;
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
ImgTree::ImgTree(const PNG &img)
{
    Stats s(img);
    root = BuildNode(s, 0, 0, img.height() - 1, img.width() - 1);
    imgheight = img.height();
    imgwidth = img.width();
}

/**
 *  Releases all heap memory associated with this tree, restoring it to an "empty tree" state.
 *  Will be useful to define a recursive helper function for this.
 */
void ImgTree::Clear()
{
    Clear(root);
    root = nullptr;
}

/**
 *  Copies the supplied parameter tree into this tree. Does not free any memory.
 *  Called by the copy constructor and operator=.
 *  Will be useful to define a recursive helper function for this.
 *  HINT: the helper should allocate a new node, and return a pointer to the allocated node.
 *        See the documention for BuildNode - this should work similarly.
 */
void ImgTree::Copy(const ImgTree &other)
{
    imgwidth = other.imgwidth;
    imgheight = other.imgheight;
    root = Copy(other.root);
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
ImgTreeNode *ImgTree::BuildNode(Stats &s, unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt)
{
    // Calculate the average color for the current region
    RGBAPixel avg = s.GetAvg(upr, lft, lwr, rt);

    // Create the new node with the calculated average color
    ImgTreeNode *node = new ImgTreeNode(upr, lft, lwr, rt, avg);

    // Base case: if the region is a single pixel, return the node
    if (upr == lwr && lft == rt)
    {
        return node;
    }

    // Determine the best split and create child nodes
    unsigned int splitCoordinate;
    bool verticalSplit;

    if ((rt - lft) >= (lwr - upr))
    {
        // Vertical split
        verticalSplit = true;
        splitCoordinate = FindBestSplit(s, upr, lft, lwr, rt, verticalSplit);
    }
    else
    {
        // Horizontal split
        verticalSplit = false;
        splitCoordinate = 
        (s, upr, lft, lwr, rt, verticalSplit);
    }

    // Recursively build child nodes
    if (verticalSplit)
    {
        node->A = BuildNode(s, upr, lft, lwr, splitCoordinate);
        node->B = BuildNode(s, upr, splitCoordinate + 1, lwr, rt);
    }
    else
    {
        node->A = BuildNode(s, upr, lft, splitCoordinate, rt);
        node->B = BuildNode(s, splitCoordinate + 1, lft, lwr, rt);
    }

    return node;
}

/**
 *  Produces a PNG of appropriate dimensions and paints every leaf node's rectangle
 *  into the appropriate area of the PNG.
 *  May be called on pruned trees.
 *  @param scaling factor for how large to render the image. Assume this is >= 1.
 *  @return fully-colored PNG, painted from the tree's leaf node data
 */
PNG ImgTree::Render(unsigned int scale) const
{
    PNG completePic;
    completePic.resize(imgwidth * scale, imgheight * scale);
    renderLeaf(completePic, scale, root);
    return completePic;
}

/**
 *  Rearranges a tree's internal pointers and node content so that its image data
 *  appears flipped horizontally when rendered.
 *  Beware that the tree may or may not have been pruned!
 *  Will be useful to define a recursive helper function for this.
 */
void ImgTree::FlipHorizontal()
{
    FlipHorizontalR(root);
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
void ImgTree::Prune(double pct, double tol)
{
    PruneLeavesR(pct, tol, root);
}

/**
 *  Counts the number of leaf nodes in the tree.
 *  Will be useful to define a recursive helper function for this.
 */
unsigned int ImgTree::CountLeaves() const
{
    return CountLeavesR(root);
}

/**
 *  ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS BELOW
 */

void ImgTree::renderLeaf(PNG &targetPic, unsigned int scale, ImgTreeNode *subTree) const
{
    if (subTree == nullptr)
    {
        return;
    }
    // Base case: leaf node, paint on the picture.
    if (subTree->A == nullptr && subTree->B == nullptr)
    {
        for (size_t x = subTree->left; x <= subTree->right; x++)
        {
            for (size_t y = subTree->upper; y <= subTree->lower; y++)
            {
                RGBAPixel refPix = subTree->avg;
                for (size_t targetX = x * scale; targetX < (x + 1) * scale; targetX++)
                {
                    for (size_t targetY = y * scale; targetY < (y + 1) * scale; targetY++)
                    {
                        *targetPic.getPixel(targetX, targetY) = refPix;
                    }
                }
            }
        }
        return;
    }
    // recurvise step: get to the leaf node.
    if (subTree->A != nullptr)
    {
        renderLeaf(targetPic, scale, subTree->A);
    }
    if (subTree->B != nullptr)
    {
        renderLeaf(targetPic, scale, subTree->B);
    }
}

unsigned int ImgTree::FindBestSplit(Stats &s, unsigned int upr, unsigned int lft, unsigned int lwr, unsigned int rt, bool vertical)
{
    // Find the best split coordinate that minimizes the sum of squared deviations
    unsigned int bestSplit = (vertical ? lft - 1 : upr - 1);
    double minSumSq = 1e12;

    if (vertical)
    {
        for (unsigned int i = lft; i < rt; ++i)
        {
            double sumSqLeft = s.GetSumSqDev(upr, lft, lwr, i);
            double sumSqRight = s.GetSumSqDev(upr, i + 1, lwr, rt);
            double sumSq = sumSqLeft + sumSqRight;
            if (sumSq < minSumSq)
            {
                minSumSq = sumSq;
                bestSplit = i;
                continue;
            }
            if (sumSq == minSumSq)
            {
                int half = -1;
                int width = rt - lft + 1;
                if (width % 2 == 0)
                {
                    half = width / 2 + lft - 1;
                }
                else
                {
                    half = width / 2 + lft;
                }

                if (abs(half - (int)i) < abs(half - (int)bestSplit))
                {
                    bestSplit = i;
                }
            }
        }
    }
    else
    {
        for (unsigned int i = upr; i < lwr; ++i)
        {
            double sumSqUpper = s.GetSumSqDev(upr, lft, i, rt);
            double sumSqLower = s.GetSumSqDev(i + 1, lft, lwr, rt);
            double sumSq = sumSqUpper + sumSqLower;
            if (sumSq < minSumSq)
            {
                minSumSq = sumSq;
                bestSplit = i;
            }
            if (sumSq == minSumSq)
            {
                int half = -1;
                int height = lwr - upr + 1;
                if (height % 2 == 0)
                {
                    half = height / 2 + lft - 1;
                }
                else
                {
                    half = height / 2 + lft;
                }

                if (abs(half - (int)i) < abs(half - (int)bestSplit))
                {
                    bestSplit = i;
                }
            }
        }
    }

    return bestSplit;
}

void ImgTree::FlipHorizontalR(ImgTreeNode *subTree)
{
    if (subTree == nullptr)
    {
        return;
    }
    if (subTree->A == nullptr && subTree->B == nullptr)
    {
        subTree->left = imgwidth - subTree->left - 1;
        subTree->right = imgwidth - subTree->right - 1;
        swap(subTree->left, subTree->right);
        return;
    }
    else
    {
        FlipHorizontalR(subTree->A);
        FlipHorizontalR(subTree->B);
    }
}

int ImgTree::CountLeavesR(ImgTreeNode *subTree) const
{
    if (subTree->A == nullptr && subTree->B == nullptr)
    {
        return 1;
    }
    else
    {
        return CountLeavesR(subTree->A) + CountLeavesR(subTree->B);
    }
}

ImgTreeNode* ImgTree::Copy(ImgTreeNode *const &originSubTree)
{
    if (originSubTree == nullptr)
    {
        return nullptr;
    }
    else
    {
        ImgTreeNode *subTree= new ImgTreeNode(originSubTree->upper,
                                                originSubTree->left,
                                                originSubTree->lower,
                                                originSubTree->right,
                                                originSubTree->avg);
        subTree->A = Copy(originSubTree->A);
        subTree->B = Copy(originSubTree->B);
        return subTree;
    }
}

void ImgTree::Clear(ImgTreeNode *subTree)
{
    if (subTree == nullptr)
    {
        return;
    }
    Clear(subTree->A);
    Clear(subTree->B);
    delete subTree;
    subTree = nullptr;
}

void ImgTree::PruneLeavesR(double pct, double tol, ImgTreeNode *subTree)
{
    // trivial case
    if (subTree == nullptr)
    {
        return;
    }

    size_t tolLeaves = CountTolerantLeavesR(tol, subTree, subTree->avg);
    size_t totalLeaves = CountLeavesR(subTree);
    if (totalLeaves == 0)
    {
        return;
    }

    double tolPct = (double)tolLeaves / (double)totalLeaves * 100.0;
    if (tolPct >= pct)
    {
        Clear(subTree->A);
        Clear(subTree->B);
        subTree->A = nullptr;
        subTree->B = nullptr;
    }

    PruneLeavesR(pct, tol, subTree->A);
    PruneLeavesR(pct, tol, subTree->B);
}

int ImgTree::CountTolerantLeavesR(double tol, ImgTreeNode *subTree, RGBAPixel avg)
{
    if (subTree == nullptr)
    {
        return 0;
    }
    if (subTree->A == nullptr && subTree->B == nullptr)
    {
        // check tolerance;
        double colorDist = avg.dist(subTree->avg);
        if (colorDist <= tol)
        {
            return 1;
        }
        return 0;
    }
    return CountTolerantLeavesR(tol, subTree->A, avg) + CountTolerantLeavesR(tol, subTree->B, avg);
}
/**
 *  @file imgtree-private.h
 *  @description student-defined functions for binary tree class storing image data for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  SUBMIT THIS FILE TO PRAIRIELEARN, EVEN IF YOU DO NOT ADD ANYTHING TO IT
 * 
 *  Usage: As this file is included entirely into imgtree.h by the #include statement on line 214
 *  you only need to write the function signature,
 *  e.g.
 *  void MyOwnSpecialFunction(int fave_num, ImgTreeNode* nd);
 * 
 *  and add/complete your implementation in imgtree.cpp
 */

    
    void renderLeaf(PNG &targetPic, unsigned int scale, ImgTreeNode* subTree) const;
    unsigned int FindBestSplit(Stats& s, unsigned int upr, unsigned int lft, 
      unsigned int lwr, unsigned int rt, bool vertical);
    void FlipHorizontalR(ImgTreeNode* subTree);
    int CountLeavesR(ImgTreeNode *subTree) const;
    ImgTreeNode* Copy(ImgTreeNode* const &originSubTree);
    void Clear(ImgTreeNode *subTree);
    void PruneLeavesR(double pct, double tol, ImgTreeNode* root);
    int CountTolerantLeavesR(double tol, ImgTreeNode *subTree, RGBAPixel avg);
/**
 *  @file pa3.cpp
 *  @description basic test cases for CPSC 221 PA3
 *  @author CPSC 221
 *
 *  You are highly recommended to add your own test cases and images to this file.
 *
 *  USAGE: run with a single number as the command-line parameter, e.g.:
 *  ./pa3 3
 *
 *  Set image information in the SetImagePaths function at the bottom of this file.
 *
 *  THIS FILE WILL NOT BE SUBMITTED TO PRAIRIELEARN
 */

#include <iostream>
#include <string>

#include "imgtree.h"

using namespace cs221util;
using namespace std;

// global strings for images
string input_img_path;
string output_img_path;
string soln_img_path;

// Test function declarations
void TestStats();
void TestImgTreeBuildRender();
void TestImgTreeFlipHorizontal();
void TestImgTreeCountLeavesPrune();
void TestCountLeaves();
void TestCopy();

// Test support function
void SetImagePaths(int imgnum);

// Program entry point
int main(int argc, char *argv[])
{

	// provide one command-line argument as a number in the range of [0, 6] to specify the test image used
	int image_number = 1; // default image number
	// set image_number from first command-line argument
	if (argc > 1)
		image_number = atoi(argv[1]);

	SetImagePaths(image_number);

	// // add or uncomment calls to test functions as needed
	// TestStats();
	// TestImgTreeBuildRender();
	// TestImgTreeFlipHorizontal();
	// TestCountLeaves();
	TestCopy();
	// TestImgTreeCountLeavesPrune();

	return 0;
}

// Test function implementations
void TestStats()
{
	cout << "Entered TestStats..." << endl;

	cout << "Creating a input PNG... ";
	// make a 3x3 PNG with all alpha = 1.0, and RGB values as:
	// 12   3   6
	//  8  14   1
	//  9   7  11
	PNG inputimg;
	cout << "inputimg name:" << input_img_path;

	inputimg.readFromFile(input_img_path);

	// RGBAPixel *px;
	// px = testpng.getPixel(0, 0);
	// *px = RGBAPixel(12, 12, 12, 1.0);
	// px = testpng.getPixel(1, 0);
	// *px = RGBAPixel(3, 3, 3, 1.0);
	// px = testpng.getPixel(2, 0);
	// *px = RGBAPixel(6, 6, 6, 1.0);
	// px = testpng.getPixel(0, 1);
	// *px = RGBAPixel(8, 8, 8, 1.0);
	// px = testpng.getPixel(1, 1);
	// *px = RGBAPixel(14, 14, 14, 1.0);
	// px = testpng.getPixel(2, 1);
	// *px = RGBAPixel(1, 1, 1, 1.0);
	// px = testpng.getPixel(0, 2);
	// *px = RGBAPixel(9, 9, 9, 1.0);
	// px = testpng.getPixel(1, 2);
	// *px = RGBAPixel(7, 7, 7, 1.0);
	// px = testpng.getPixel(2, 2);
	// *px = RGBAPixel(11, 11, 11, 1.0);

	cout << "done." << endl;
	cout << "Creating Stats object... ";

	Stats st(inputimg);

	cout << "done." << endl;

	// cout << "Getting average color of the 2x2 square in the lower right corner..." << endl;
	// cout << "Expected value:\t" << RGBAPixel(8, 8, 8, 1.0) << endl;
	// cout << "Your value:\t" << st.GetAvg(1, 1, 2, 2) << endl;
	unsigned long totalR = 0;
	unsigned long totalG = 0;
	unsigned long totalB = 0;
	double totalA = 0.0;

	unsigned long totalSqR = 0;
	unsigned long totalSqG = 0;
	unsigned long totalSqB = 0;
	double totalSqA = 0.0;

	cout << "Getting total sum square difference from average color of the entire img..." << endl;
	size_t w = inputimg.width();
	size_t h = inputimg.height();
	for (unsigned long x = 1; x < w; x++)
	{
		for (unsigned long y = 0; y < h; y++)
		{
			unsigned long r = (unsigned int)inputimg.getPixel(x, y)->r;
			unsigned long g = (unsigned int)inputimg.getPixel(x, y)->g;
			unsigned long b = (unsigned int)inputimg.getPixel(x, y)->b;
			double a = inputimg.getPixel(x, y)->a * 255.0;

			unsigned long rSq = r * r;
			unsigned long gSq = g * g;
			unsigned long bSq = b * b;
			double aSq = a * a;

			totalR += r;
			totalG += g;
			totalB += b;
			totalA += a;
			totalSqR += rSq;
			totalSqG += gSq;
			totalSqB += bSq;
			totalSqA += aSq;
		}
	}

	unsigned int area = (w-1) * h;

	double SqDevR = totalSqR - totalR * totalR / (double) area;
	double SqDevG = totalSqG - totalG * totalG / (double) area;
	double SqDevB = totalSqB - totalB * totalB / (double) area;
	double SqDevA = totalSqA - totalA * totalA / (double) area;

	double SumSqDev = SqDevR + SqDevG + SqDevB + SqDevA;

	cout << "expected sumR = " << totalR << endl;
	cout << "actual sumR = " << st.GetColorSum('r', 0, 1, h - 1, w - 1) << endl;
	cout << "expected sumG = " << totalG << endl;
	cout << "actual sumG = " << st.GetColorSum('g', 0, 1, h - 1, w - 1) << endl;
	cout << "expected sumB = " << totalB << endl;
	cout << "actual sumB = " << st.GetColorSum('b', 0, 1, h - 1, w - 1) << endl;
	cout << "expected sumA = " << totalA << endl;
	cout << "actual sumA = " << st.GetAlphaSum(0, 1, h - 1, w - 1) << endl;
	cout << "difference" << st.GetAlphaSum(0, 1, h - 1, w - 1) - totalA << endl;

	cout << "expected sumSqR = " << totalSqR << endl;
	cout << "actual sumSqR = " << st.GetColorSumSq('r',0,1,h-1,w-1) << endl;
	cout << "expected sumSqG = " << totalSqG << endl;
	cout << "actual sumSqG = " << st.GetColorSumSq('g',0,1,h-1,w-1) << endl;
	cout << "expected sumSqB = " << totalSqB << endl;
	cout << "actual sumSqB = " << st.GetColorSumSq('b',0,1,h-1,w-1) << endl;
	cout << "expected sumSqA = " << totalSqA << endl;
	cout << "actual sumSqA = " << st.GetAlphaSumSq(0, 1, h - 1, w - 1) << endl;
	cout << "difference" << st.GetAlphaSumSq(0, 1, h - 1, w - 1) - totalSqA << endl;

	cout << "Expected area:" << (w-1) * h << endl;
	cout << "actual area " << st.GetRectangleArea(0, 1, h - 1, w - 1) << endl;

	cout << "Expected value:\t" << SumSqDev << endl;
	cout << "Your value:\t" << st.GetSumSqDev(0, 1, h - 1, w - 1) << endl;
	cout << "difference" << st.GetSumSqDev(0, 1, h - 1, w - 1) - SumSqDev << endl;

	cout << "Leaving TestStats...\n"
		 << endl;
}

void TestImgTreeBuildRender()
{
	cout << "Entered TestImgTreeBuildRender..." << endl;

	PNG inputimg, solnimg;
	cout << "solution img name:" << soln_img_path;
	inputimg.readFromFile(input_img_path);
	solnimg.readFromFile(soln_img_path + "-r1.png");

	cout << "Constructing ImgTree from input image...";
	ImgTree mytree(inputimg);
	cout << "done." << endl;

	cout << "Rendering ImgTree to output at 1x scale...";
	PNG outputimg = mytree.Render(1);
	cout << "done." << endl;

	cout << "Saving rendered PNG to file...";
	outputimg.writeToFile(output_img_path + "-r1.png");
	cout << "done." << endl;

	cout << "Comparing your image and expected output...";
	if (outputimg == solnimg)
	{
		cout << "Image data match." << endl;
	}
	else
	{
		cout << "Image data mismatch." << endl;
		cout << "Expected: " << solnimg << endl;
		cout << "Yours:    " << outputimg << endl;
	}

	cout << "Leaving TestImgTreeBuildRender...\n"
		 << endl;
}

void TestImgTreeFlipHorizontal()
{
	cout << "Entered TestImgTreeFlipHorizontal..." << endl;

	PNG inputimg, solnimg;
	inputimg.readFromFile(input_img_path);
	solnimg.readFromFile(soln_img_path + "-fh-r1.png");

	cout << "Constructing ImgTree from input image...";
	ImgTree mytree(inputimg);
	cout << "done." << endl;

	cout << "Calling FlipHorizontal...";
	mytree.FlipHorizontal();
	cout << "done." << endl;

	cout << "Rendering flipped ImgTree to output...";
	PNG outputimg = mytree.Render(1);
	cout << "done." << endl;

	cout << "Saving rendered PNG to file...";
	outputimg.writeToFile(output_img_path + "-fh-r1.png");
	cout << "done." << endl;

	cout << "Comparing your image and expected output...";
	if (outputimg == solnimg)
	{
		cout << "Image data match." << endl;
	}
	else
	{
		cout << "Image data mismatch." << endl;
		cout << "Expected: " << solnimg << endl;
		cout << "Yours:    " << outputimg << endl;
	}

	cout << "Leaving TestImgTreeFlipHorizontal...\n"
		 << endl;
}

void TestImgTreeCountLeavesPrune()
{
	cout << "Entered TestImgTreePrune..." << endl;

	PNG inputimg, solnimg;
	inputimg.readFromFile(input_img_path);
	solnimg.readFromFile(soln_img_path + "-prune-pct85-tol002-r1.png");
	cout << "solution img name:" << soln_img_path;

	cout << "Constructing ImgTree from input image...";
	ImgTree mytree(inputimg);
	cout << "done." << endl;

	cout << "Counting leaves in the tree... ";
	unsigned int numleaves = mytree.CountLeaves();
	cout << "Tree contains " << numleaves << " leaves." << endl;

	cout << "Calling Prune with pct=85 and tol=0.02...";
	mytree.Prune(85, 0.02);

	cout << "Counting leaves in the pruned tree... ";
	unsigned int numleavespruned = mytree.CountLeaves();
	cout << "Tree contains " << numleavespruned << " leaves." << endl;

	
	cout << "Calling flip horizontal";
	mytree.FlipHorizontal();
	cout << "done." << endl;

	cout << "Counting leaves in the flipped tree... ";
	numleavespruned = mytree.CountLeaves();
	cout << "Tree contains " << numleavespruned << " leaves." << endl;

	cout << "Rendering pruned ImgTree to output...";
	PNG outputimg = mytree.Render(1);
	cout << "done." << endl;

	cout << "Saving rendered PNG to file...";
	outputimg.writeToFile(output_img_path + "-prune-pct85-tol002-r1.png");
	cout << "done." << endl;

	cout << "Comparing your image and expected output...";
	if (outputimg == solnimg)
	{
		cout << "Image data match." << endl;
	}
	else
	{
		cout << "Image data mismatch." << endl;
		cout << "Expected: " << solnimg << endl;
		cout << "Yours:    " << outputimg << endl;
	}

	cout << "Leaving TestImgTreePrune...\n"
		 << endl;
}

void SetImagePaths(int imgnum)
{
	switch (imgnum)
	{
	case 0:
		input_img_path = "images-orig/green-1x1.png";
		output_img_path = "images-output/green-1x1";
		soln_img_path = "images-soln/ex-green-1x1";
		break;
	case 1:
		input_img_path = "images-orig/rgb-3x1.png";
		output_img_path = "images-output/rgb-3x1";
		soln_img_path = "images-soln/ex-rgb-3x1";
		break;
	case 2:
		input_img_path = "images-orig/mix-2x5.png";
		output_img_path = "images-output/mix-2x5";
		soln_img_path = "images-soln/ex-mix-2x5";
		break;
	case 3:
		input_img_path = "images-orig/mix-3x3.png";
		output_img_path = "images-output/mix-3x3";
		soln_img_path = "images-soln/ex-mix-3x3";
		break;
	case 4:
		input_img_path = "images-orig/pruneto16leaves-8x5.png";
		output_img_path = "images-output/pruneto16leaves-8x5";
		soln_img_path = "images-soln/ex-pruneto16leaves-8x5";
		break;
	case 5:
		input_img_path = "images-orig/malachi-60x87.png";
		output_img_path = "images-output/malachi-60x87";
		soln_img_path = "images-soln/ex-malachi-60x87";
		break;
	case 6:
		input_img_path = "images-orig/kkkk-kmnn-960x540.png";
		output_img_path = "images-output/kkkk-kmnn-960x540";
		soln_img_path = "images-soln/ex-kkkk-kmnn-960x540";
		break;
	default:
		input_img_path = "images-orig/green-1x1.png";
		output_img_path = "images-output/green-1x1";
		soln_img_path = "images-soln/ex-green-1x1";
		break;
	}
}

void TestCountLeaves()
{
	cout << "Entered TestCountLeaves..." << endl;

	PNG inputimg;
	cout << "input img name:" << input_img_path << endl;
	inputimg.readFromFile(input_img_path);

	cout << "Constructing ImgTree from input image...";
	ImgTree mytree(inputimg);
	cout << "done." << endl;

	cout << "count leaves..." << endl;
	size_t count = mytree.CountLeaves();
	cout << "done." << endl;

	cout << "Comparing your count and expected output...";
	if (count == inputimg.height() * inputimg.width())
	{
		cout << "count correct." << endl;
	}
	else
	{
		cout << "Image data mismatch." << endl;
		cout << "Expected: " << inputimg.height() * inputimg.width() << endl;
		cout << "Yours:    " << count << endl;
	}

	cout << "Leaving TestCountLeaves...\n"
		 << endl;
}

void TestCopy()
{
	cout << "Entered TestCopy..." << endl;

	PNG inputimg, solnimg;
	cout << "solution img name:" << soln_img_path << endl;
	inputimg.readFromFile(input_img_path);
	solnimg.readFromFile(soln_img_path + "-r1.png");

	cout << "Constructing ImgTree from input image...";
	ImgTree mytree(inputimg);
	cout << "done." << endl;

	cout << "total node"  << mytree.CountLeaves()<< endl;

	cout << "Copy ImgTree" << endl;
	ImgTree mytreeCopy;
	mytreeCopy = mytree;
	mytreeCopy = mytree;
	mytreeCopy = mytree;
	cout << "done." << endl;

	cout << "Rendering ImgTree to output at 1x scale...";
	PNG outputimg = mytreeCopy.Render(1);
	cout << "done." << endl;

	cout << "Saving rendered PNG to file...";
	outputimg.writeToFile(output_img_path + "-r1.png");
	cout << "done." << endl;

	cout << "Comparing your image and expected output...";
	if (outputimg == solnimg)
	{
		cout << "Image data match." << endl;
	}
	else
	{
		cout << "Image data mismatch." << endl;
		cout << "Expected: " << solnimg << endl;
		cout << "Yours:    " << outputimg << endl;
	}

	cout << "Leaving TestImgTreeBuildRender...\n"
		 << endl;
}
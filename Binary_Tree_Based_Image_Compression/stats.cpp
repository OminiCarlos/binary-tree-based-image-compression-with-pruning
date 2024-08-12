/**
 *  @file stats.cpp
 *  @description implementation of a stats class for rapid calculation of color averages
 *   and total color differences in CPSC 221 PA3
 *  @author CPSC 221
 *
 *  SUBMIT THIS FILE TO PRAIRIELEARN
 */

#include "stats.h"

/**
 *  Computes/retrieves the sum of a single color channel in a defined rectangular region
 *  @pre channel is a valid channel identifier
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param channel - one of 'r', 'g', or 'b'
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return the sum of the appropriate color channel values in the defined rectangular area
 */
unsigned long Stats::GetColorSum(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  vector<vector<unsigned long>> *colorptr = nullptr;
  if (channel == 'r')
  {
    colorptr = &sumR;
  }
  else if (channel == 'g')
  {
    colorptr = &sumG;
  }
  else if (channel == 'b')
  {
    colorptr = &sumB;
  }

  unsigned long colorSum = 0;

  if (upper == 0 && left == 0)
  {
    colorSum = (*colorptr)[right][lower];
  }
  else if (upper == 0)
  {
    colorSum = (*colorptr)[right][lower] - (*colorptr)[left - 1][lower];
  }
  else if (left == 0)
  {
    colorSum = (*colorptr)[right][lower] - (*colorptr)[right][upper - 1];
  }
  else
  {
    colorSum = (*colorptr)[right][lower] - (*colorptr)[right][upper - 1] - (*colorptr)[left - 1][lower] + (*colorptr)[left - 1][upper - 1];
  }

  return colorSum;
}

/**
 *  Computes/retrieves the sum of alpha values in a defined rectangular region
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return the sum of the alpha values in the defined rectangular area
 */
double Stats::GetAlphaSum(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  double alphaSum = 0.0;
  if (upper == 0 && left == 0)
  {
    alphaSum = sumA[right][lower];
  }
  else if (upper == 0)
  {
    alphaSum = sumA[right][lower] - sumA[left - 1][lower];
  }
  else if (left == 0)
  {
    alphaSum = sumA[right][lower] - sumA[right][upper - 1];
  }
  else
  {
    alphaSum = sumA[right][lower] - sumA[right][upper - 1] - sumA[left - 1][lower] + sumA[left - 1][upper - 1];
  }
  return alphaSum;
}

/**
 *  Computes/retrieves the squared sum of a single color channel in a defined rectangular region
 *  @pre channel is a valid channel identifier
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param channel - one of 'r', 'g', or 'b'
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return the squared sum of the appropriate color channel values in the defined rectangular area
 */
unsigned long Stats::GetColorSumSq(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  vector<vector<unsigned long>> *colorptr = nullptr;
  if (channel == 'r')
  {
    colorptr = &sumSqR;
  }
  else if (channel == 'g')
  {
    colorptr = &sumSqG;
  }
  else if (channel == 'b')
  {
    colorptr = &sumSqB;
  }

  unsigned long colorSumSq = 0;

  if (upper == 0 && left == 0)
  {
    colorSumSq = (*colorptr)[right][lower];
  }
  else if (upper == 0)
  {
    colorSumSq = (*colorptr)[right][lower] - (*colorptr)[left - 1][lower];
  }
  else if (left == 0)
  {
    colorSumSq = (*colorptr)[right][lower] - (*colorptr)[right][upper - 1];
  }
  else
  {
    colorSumSq = (*colorptr)[right][lower] - (*colorptr)[right][upper - 1] - (*colorptr)[left - 1][lower] + (*colorptr)[left - 1][upper - 1];
  }

  return colorSumSq;
}

/**
 *  Computes/retrieves the squared sum of alpha values in a defined rectangular region
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return the squared sum of the alpha values in the defined rectangular area
 */
double Stats::GetAlphaSumSq(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  double alphaSumSq = 0.0;
  if (upper == 0 && left == 0)
  {
    alphaSumSq = sumSqA[right][lower];
  }
  else if (upper == 0)
  {
    alphaSumSq = sumSqA[right][lower] - sumSqA[left - 1][lower];
  }
  else if (left == 0)
  {
    alphaSumSq = sumSqA[right][lower] - sumSqA[right][upper - 1];
  }
  else
  {
    alphaSumSq = sumSqA[right][lower] - sumSqA[right][upper - 1] - sumSqA[left - 1][lower] + sumSqA[left - 1][upper - 1];
  }
  return alphaSumSq;
}

/**
 *  Simple function to compute the number of pixels in a defined rectangular region
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return the area of the defined rectangular area, in pixels
 */
unsigned int Stats::GetRectangleArea(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  unsigned int area = (lower - upper + 1) * (right - left + 1);
  return area; // REPLACE THIS STUB
}

/**
 *  Parameterized constructor
 *  Builds the channel sum and squared sum vectors from the supplied input image.
 *  Each channel sum vector's entry (x,y) will contain the sum of their respective
 *  color channels of all pixels in the rectangular image region bounded by (0,0) and (x,y).
 *  Likewise, each channel squared sum vector's entry (x,y) will contain the squared sum of their
 *  respective color channels of all pixels in the rectangular image region bounded by (0,0) and (x,y).
 *
 *  ***DON'T FORGET TO PRE-MULTIPLY THE ALPHA CHANNEL***
 *
 *  @param img - input image from which the channel sum vectors will be populated
 */
Stats::Stats(const PNG &img)
{
  // initialize sum vectors;
  sumR.resize(img.width());
  for (auto &col : sumR)
  {
    col.resize(img.height());
  }
  sumG.resize(img.width());
  for (auto &col : sumG)
  {
    col.resize(img.height());
  }
  sumB.resize(img.width());
  for (auto &col : sumB)
  {
    col.resize(img.height());
  }
  sumA.resize(img.width());
  for (auto &col : sumA)
  {
    col.resize(img.height());
  }

  // initalize sqrSum vectors
  sumSqR.resize(img.width());
  for (auto &col : sumSqR)
  {
    col.resize(img.height());
  }
  sumSqG.resize(img.width());
  for (auto &col : sumSqG)
  {
    col.resize(img.height());
  }
  sumSqB.resize(img.width());
  for (auto &col : sumSqB)
  {
    col.resize(img.height());
  }
  sumSqA.resize(img.width());
  for (auto &col : sumSqA)
  {
    col.resize(img.height());
  }

  // populate sumR vector; read 4 pixel to the left and above of x,y and calculate the prefix sum
  for (size_t x = 0; x < img.width(); x++)
  {
    for (size_t y = 0; y < img.height(); y++)
    {
      unsigned long r = static_cast<unsigned long> (img.getPixel(x, y)->r);
      unsigned long g = static_cast<unsigned long> (img.getPixel(x, y)->g);
      unsigned long b = static_cast<unsigned long> (img.getPixel(x, y)->b);
      double a = static_cast<double>(img.getPixel(x, y)->a) * 255.0;
      unsigned long rSq = r * r;
      unsigned long gSq = g * g;
      unsigned long bSq = b * b;
      double aSq = a * a;

      if (x == 0 && y == 0)
      {
        sumR[x][y] = r;
        sumG[x][y] = g;
        sumB[x][y] = b;
        sumA[x][y] = a;
        sumSqR[x][y] = rSq;
        sumSqG[x][y] = gSq;
        sumSqB[x][y] = bSq;
        sumSqA[x][y] = aSq;
      }
      else if (x == 0)
      {

        sumR[x][y] = r + sumR[x][y - 1];
        sumG[x][y] = g + sumG[x][y - 1];
        sumB[x][y] = b + sumB[x][y - 1];
        sumA[x][y] = a + sumA[x][y - 1];
        sumSqR[x][y] = sumSqR[x][y - 1] + rSq;
        sumSqG[x][y] = sumSqG[x][y - 1] + gSq;
        sumSqB[x][y] = sumSqB[x][y - 1] + bSq;
        sumSqA[x][y] = sumSqA[x][y - 1] + aSq;
      }
      else if (y == 0)
      {

        sumR[x][y] = r + sumR[x - 1][y];
        sumG[x][y] = g + sumG[x - 1][y];
        sumB[x][y] = b + sumB[x - 1][y];
        sumA[x][y] = a + sumA[x - 1][y];
        sumSqR[x][y] = sumSqR[x - 1][y] + rSq;
        sumSqG[x][y] = sumSqG[x - 1][y] + gSq;
        sumSqB[x][y] = sumSqB[x - 1][y] + bSq;
        sumSqA[x][y] = sumSqA[x - 1][y] + aSq;
      }
      else
      {

        sumR[x][y] = sumR[x - 1][y] + sumR[x][y - 1] - sumR[x - 1][y - 1] + r;
        sumG[x][y] = sumG[x - 1][y] + sumG[x][y - 1] - sumG[x - 1][y - 1] + g;
        sumB[x][y] = sumB[x - 1][y] + sumB[x][y - 1] - sumB[x - 1][y - 1] + b;
        sumA[x][y] = sumA[x - 1][y] + sumA[x][y - 1] - sumA[x - 1][y - 1] + a;
        sumSqR[x][y] = sumSqR[x - 1][y] + sumSqR[x][y - 1] - sumSqR[x - 1][y - 1] + rSq;
        sumSqG[x][y] = sumSqG[x - 1][y] + sumSqG[x][y - 1] - sumSqG[x - 1][y - 1] + gSq;
        sumSqB[x][y] = sumSqB[x - 1][y] + sumSqB[x][y - 1] - sumSqB[x - 1][y - 1] + bSq;
        sumSqA[x][y] = sumSqA[x - 1][y] + sumSqA[x][y - 1] - sumSqA[x - 1][y - 1] + aSq;
      }
    }
  }
}

/**
 *  Computes/retrieves the average color of all pixels contained in the rectangle
 *  bounded by upper, left, lower, and right. Fractional values should be
 *  truncated/rounded down for assignment into integer variables.
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return pixel containing the average color of the pixels in the defined rectangle
 */
RGBAPixel Stats::GetAvg(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  size_t totalR = GetColorSum('r', upper, left, lower, right);
  size_t totalG = GetColorSum('g', upper, left, lower, right);
  size_t totalB = GetColorSum('b', upper, left, lower, right);
  double totalA = GetAlphaSum(upper, left, lower, right);
  size_t n = GetRectangleArea(upper, left, lower, right);

  unsigned char avgR = totalR / n;
  unsigned char avgG = totalG / n;
  unsigned char avgB = totalB / n;
  double avgA = totalA / n / 255.0;

  RGBAPixel avgPix(avgR, avgG, avgB, avgA);
  return avgPix;
}

/**
 *  Computes the total sum squared difference from the mean, for the specified rectangle.
 *  Each channel's sum squared difference is computed separately, and then added to form the total.
 *
 *  Note that using the GetAvg function in computing the sum squared difference will result in
 *  accumulation of error especially with larger rectangles.
 *  You should use more precise computation of the average color for this function's intermediate steps.
 *
 *  @pre upper, left, lower, and right are valid image coordinates
 *  @param upper - y-coordinate of the upper edge of the rectangular region
 *  @param left - x-coordinate of the left side of the rectangular region
 *  @param lower - y-coordinate of the lower edge of the rectangular region
 *  @param right - x-coordinate of the right side of the rectangular region
 *  @return total sum of squared deviations from the mean, over all color channels.
 */
double Stats::GetSumSqDev(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right)
{
  double n = GetRectangleArea(upper, left, lower, right);

  unsigned long totalR = GetColorSum('r', upper, left, lower, right);
  unsigned long totalG = GetColorSum('g', upper, left, lower, right);
  unsigned long totalB = GetColorSum('b', upper, left, lower, right);
  double totalA = GetAlphaSum(upper, left, lower, right);

  unsigned long totalSqR = GetColorSumSq('r', upper, left, lower, right);
  unsigned long totalSqG = GetColorSumSq('g', upper, left, lower, right);
  unsigned long totalSqB = GetColorSumSq('b', upper, left, lower, right);
  double totalSqA = GetAlphaSumSq(upper, left, lower, right);

  double sqDiffR = totalSqR - totalR * totalR / n;
  double sqDiffG = totalSqG - totalG * totalG / n; 
  double sqDiffB = totalSqB - totalB * totalB / n;
  double sqDiffA = totalSqA - totalA * totalA / n;

  double sumSqDev = sqDiffR + sqDiffG + sqDiffB + sqDiffA;
  return sumSqDev; 
}
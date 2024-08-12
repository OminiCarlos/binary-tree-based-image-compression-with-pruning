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
unsigned long Stats::GetColorSum(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {
  vector<vector<unsigned long>>* targetVector;

  if (channel == 'r') {
    targetVector = &sumR;
  } else if (channel == 'g') {
    targetVector = &sumG;
  } else if (channel == 'b') {
    targetVector = &sumB;
    
  }

  unsigned long result = (*targetVector)[lower][right];

  if (upper > 0) {
        result = result - (*targetVector)[upper-1][right];
    }
    if (left > 0) {
        result = result - (*targetVector)[lower][left-1];
    }
    if (upper > 0 && left > 0) {
        result = result + (*targetVector)[upper-1][left-1];
    }

	return result; // REPLACE THIS STUB
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
double Stats::GetAlphaSum(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {

  double result = sumA[lower][right];
  if (upper > 0) {
    result = result - sumA[upper-1][right];
  }
  if (left > 0) {
    result = result - sumA[lower][left-1];
  }
  if (upper > 0 && left > 0) {
    result = result + sumA[upper-1][left-1];
  }

	return result; // REPLACE THIS STUB
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
unsigned long Stats::GetColorSumSq(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {
  vector<vector<unsigned long>>* targetVector;

  if (channel == 'r') {
    targetVector = &sumSqR;
  } else if (channel == 'g') {
    targetVector = &sumSqG;
  } else if (channel == 'b') {
    targetVector = &sumSqB;
  }

  unsigned long result = (*targetVector)[lower][right];

  if (upper > 0) {
    result = result - (*targetVector)[upper-1][right];
  }
  if (left > 0) {
    result = result - (*targetVector)[lower][left-1];
  }
  if (upper > 0 && left > 0) {
    result = result + (*targetVector)[upper-1][left-1];
  }

	return result; // REPLACE THIS STUB
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
double Stats::GetAlphaSumSq(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {

  double result = sumSqA[lower][right];
  if (upper > 0) {
    result = result - sumSqA[upper-1][right];
  }
  if (left > 0) {
    result = result - sumSqA[lower][left-1];
  }
  if (upper > 0 && left > 0) {
    result = result + sumSqA[upper-1][left-1];
  }
	return result; // REPLACE THIS STUB
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
unsigned int Stats::GetRectangleArea(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {
  unsigned int numPixel = (right - left + 1) * (lower - upper + 1);
	return numPixel; // REPLACE THIS STUB
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
Stats::Stats(const PNG& img) {
	// complete your implementation below

  // we want to visit each pixel of the img, 
  // read its RGBA value, and update the RGBA sums in the 2D vectors

  // Initialize sum and sumSq vectors based on image dimensions
    unsigned int width = img.width();
    unsigned int height = img.height();

    sumR.resize(height, vector<unsigned long>(width, 0));
    sumG.resize(height, vector<unsigned long>(width, 0));
    sumB.resize(height, vector<unsigned long>(width, 0));
    sumA.resize(height, vector<double>(width, 0));

    sumSqR.resize(height, vector<unsigned long>(width, 0));
    sumSqG.resize(height, vector<unsigned long>(width, 0));
    sumSqB.resize(height, vector<unsigned long>(width, 0));
    sumSqA.resize(height, vector<double>(width, 0));

    // Fill in sum and sumSq vectors
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            RGBAPixel* curr_pixel = img.getPixel(x, y);

            unsigned long r = (unsigned long)curr_pixel->r;
            unsigned long g = (unsigned long)curr_pixel->g;
            unsigned long b = (unsigned long)curr_pixel->b;
            double a = curr_pixel->a * 255.0; // Pre-multiply alpha

            unsigned long r_sq = r * r;
            unsigned long g_sq = g * g;
            unsigned long b_sq = b * b;
            double a_sq = a * a;

            // Compute cumulative sums for current pixel
            if (x == 0 && y == 0) {
                // Top-left corner
                sumR[y][x] = r;
                sumG[y][x] = g;
                sumB[y][x] = b;
                sumA[y][x] = a;

                sumSqR[y][x] = r_sq;
                sumSqG[y][x] = g_sq;
                sumSqB[y][x] = b_sq;
                sumSqA[y][x] = a_sq;
            } else if (x == 0) {
                // First column (but not top-left corner)
                sumR[y][x] = sumR[y-1][x] + r;
                sumG[y][x] = sumG[y-1][x] + g;
                sumB[y][x] = sumB[y-1][x] + b;
                sumA[y][x] = sumA[y-1][x] + a;

                sumSqR[y][x] = sumSqR[y-1][x] + r_sq;
                sumSqG[y][x] = sumSqG[y-1][x] + g_sq;
                sumSqB[y][x] = sumSqB[y-1][x] + b_sq;
                sumSqA[y][x] = sumSqA[y-1][x] + a_sq;
            } else if (y == 0) {
                // First row (but not top-left corner)
                sumR[y][x] = sumR[y][x-1] + r;
                sumG[y][x] = sumG[y][x-1] + g;
                sumB[y][x] = sumB[y][x-1] + b;
                sumA[y][x] = sumA[y][x-1] + a;

                sumSqR[y][x] = sumSqR[y][x-1] + r_sq;
                sumSqG[y][x] = sumSqG[y][x-1] + g_sq;
                sumSqB[y][x] = sumSqB[y][x-1] + b_sq;
                sumSqA[y][x] = sumSqA[y][x-1] + a_sq;
            } else {
                // General case
                sumR[y][x] = sumR[y-1][x] + sumR[y][x-1] - sumR[y-1][x-1] + r;
                sumG[y][x] = sumG[y-1][x] + sumG[y][x-1] - sumG[y-1][x-1] + g;
                sumB[y][x] = sumB[y-1][x] + sumB[y][x-1] - sumB[y-1][x-1] + b;
                sumA[y][x] = sumA[y-1][x] + sumA[y][x-1] - sumA[y-1][x-1] + a;

                sumSqR[y][x] = sumSqR[y-1][x] + sumSqR[y][x-1] - sumSqR[y-1][x-1] + r_sq;
                sumSqG[y][x] = sumSqG[y-1][x] + sumSqG[y][x-1] - sumSqG[y-1][x-1] + g_sq;
                sumSqB[y][x] = sumSqB[y-1][x] + sumSqB[y][x-1] - sumSqB[y-1][x-1] + b_sq;
                sumSqA[y][x] = sumSqA[y-1][x] + sumSqA[y][x-1] - sumSqA[y-1][x-1] + a_sq;
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
RGBAPixel Stats::GetAvg(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {
  unsigned int numPixel = (right - left + 1) * (lower - upper + 1);
  unsigned int sumR = GetColorSum('r', upper, left, lower, right);
  unsigned int sumG = GetColorSum('g', upper, left, lower, right);
  unsigned int sumB = GetColorSum('b', upper, left, lower, right);
  double sumA = GetAlphaSum(upper, left, lower, right);

  int avg_r = (int) sumR / numPixel;
  int avg_g = (int) sumG / numPixel;
  int avg_b = (int) sumB / numPixel;
  double avg_a = (double) sumA / numPixel / 255.0;

	return RGBAPixel(avg_r, avg_g, avg_b, avg_a); // REPLACE THIS STUB
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
double Stats::GetSumSqDev(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right) {

  double resultR = 0.0;
  double resultG = 0.0;
  double resultB = 0.0;
  double resultA = 0.0;
  double result = 0.0;

  double numPixel = (double) GetRectangleArea(upper, left, lower, right);

  unsigned long sumSqR = GetColorSumSq('r', upper, left, lower, right);
  unsigned long sumSqG = GetColorSumSq('g', upper, left, lower, right);
  unsigned long sumSqB = GetColorSumSq('b', upper, left, lower, right);
  double sumSqA = GetAlphaSumSq(upper, left, lower, right);

  unsigned long sumR = GetColorSum('r', upper, left, lower, right);
  unsigned long sumG = GetColorSum('g', upper, left, lower, right);
  unsigned long sumB = GetColorSum('b', upper, left, lower, right);
  double sumA = GetAlphaSum(upper, left, lower, right);

  resultR = sumSqR -sumR * sumR / numPixel;
  resultG = sumSqG -sumG * sumG / numPixel;
  resultB = sumSqB -sumB * sumB / numPixel;
  resultA = sumSqA -sumA * sumA / numPixel;



  result = resultR + resultG + resultB + resultA;

	return result; // REPLACE THIS STUB
}
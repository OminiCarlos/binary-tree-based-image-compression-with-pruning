/**
 *  @file stats.h
 *  @description definition of a stats class for rapid calculation of color averages
 *   and total color differences in CPSC 221 PA3
 * 
 *  THIS FILE WILL NOT BE SUBMITTED TO PRAIRIELEARN
 */

#ifndef _STATS_H_
#define _STATS_H_

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"

#include <utility>
#include <vector>
#include <cmath>

using namespace cs221util;
using namespace std;

class Stats {
    public:
        // 2D vectors to store color channel sums from (0,0) to (x,y)
        vector<vector<unsigned long>> sumR;
        vector<vector<unsigned long>> sumG;
        vector<vector<unsigned long>> sumB;
        vector<vector<double>> sumA; // alpha channel should be pre-multiplied by 255.0 BEFORE storing,
                                        // for proportional scaling with the RGB channels

        // 2D vectors to store color channel squared sums from (0,0) to (x,y)
        vector<vector<unsigned long>> sumSqR;
        vector<vector<unsigned long>> sumSqG;
        vector<vector<unsigned long>> sumSqB;
        vector<vector<double>> sumSqA; // alpha channel should be pre-multiplied by 255.0 BEFORE storing,
                                        // for proportional scaling with the RGB channels

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
        unsigned long GetColorSum(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

        /**
         *  Computes/retrieves the sum of alpha values in a defined rectangular region
         *  @pre upper, left, lower, and right are valid image coordinates
         *  @param upper - y-coordinate of the upper edge of the rectangular region
         *  @param left - x-coordinate of the left side of the rectangular region
         *  @param lower - y-coordinate of the lower edge of the rectangular region
         *  @param right - x-coordinate of the right side of the rectangular region
         *  @return the sum of the alpha values in the defined rectangular area
         */
        double GetAlphaSum(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

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
        unsigned long GetColorSumSq(char channel, unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

        /**
         *  Computes/retrieves the squared sum of alpha values in a defined rectangular region
         *  @pre upper, left, lower, and right are valid image coordinates
         *  @param upper - y-coordinate of the upper edge of the rectangular region
         *  @param left - x-coordinate of the left side of the rectangular region
         *  @param lower - y-coordinate of the lower edge of the rectangular region
         *  @param right - x-coordinate of the right side of the rectangular region
         *  @return the squared sum of the alpha values in the defined rectangular area
         */
        double GetAlphaSumSq(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

        /**
         *  Simple function to compute the number of pixels in a defined rectangular region
         *  @pre upper, left, lower, and right are valid image coordinates
         *  @param upper - y-coordinate of the upper edge of the rectangular region
         *  @param left - x-coordinate of the left side of the rectangular region
         *  @param lower - y-coordinate of the lower edge of the rectangular region
         *  @param right - x-coordinate of the right side of the rectangular region
         *  @return the area of the defined rectangular area, in pixels
         */
        unsigned int GetRectangleArea(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

    public:
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
        Stats(const PNG& img);

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
        RGBAPixel GetAvg(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

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
        double GetSumSqDev(unsigned int upper, unsigned int left, unsigned int lower, unsigned int right);

        /**
         *  Note that a destructor is not needed for this class, as the vectors will have their
         *  own destructors automatically called by the compiler-provided destructor when a Stats
         *  object goes out of scope.
         */
};

#endif
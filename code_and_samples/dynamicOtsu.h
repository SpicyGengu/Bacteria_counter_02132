#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
// #include <unistd.h>
#include <math.h>

#define SPLITS 5

// attempted to make otsu split into 25 sqaures, but due to numbers not

void dynamicOtsu(unsigned char inputImage[BMP_WIDTH][BMP_HEIGTH], unsigned char resultImage[BMP_WIDTH][BMP_HEIGTH])
{
    unsigned char histograms[SPLITS][SPLITS][256]; // dimension, row column, then actual values.
    int maxDistanceVert = BMP_HEIGTH / SPLITS;
    int maxDistanceHorz = BMP_WIDTH / SPLITS;
    double cumulative[SPLITS][SPLITS][256];
    double varianceHolder[SPLITS][SPLITS];
    unsigned char thresholds[SPLITS][SPLITS];
    int total_pixels = BMP_WIDTH * BMP_HEIGTH;
    int total_pixels_sector = BMP_WIDTH * BMP_HEIGTH / (SPLITS * SPLITS);
    double total_pixels_sector_float = (double)total_pixels_sector;
    int *totalPixelPointer = &total_pixels_sector;
    printf("total_pixel_sector: %d", total_pixels_sector);
    printf("float total_pixel_secotre: %f", total_pixels_sector_float);
    unsigned char startingX[SPLITS] = {maxDistanceHorz * 0, maxDistanceHorz * 1, maxDistanceHorz * 2, maxDistanceHorz * 3, maxDistanceHorz * 4};
    unsigned char startingY[SPLITS] = {maxDistanceVert * 0, maxDistanceVert * 1, maxDistanceVert * 2, maxDistanceVert * 3, maxDistanceVert * 4};

    int sum = 0;

    // Build the histograms
    int loopCount = 0;
    for (int column = 0; column < SPLITS; column++)
    {
        for (int row = 0; row < SPLITS; row++)
        {
            for (int x = 0; x < maxDistanceHorz; x++)
            {
                for (int y = 0; y < maxDistanceVert; y++)
                {
                    loopCount++;
                    histograms[row][column][inputImage[startingX[row] + x][startingY[column] + y]]++;
                }
            }
        }
    }
    printf("\n loop count: %d", loopCount);
    printf("\nMade histograms");
    printf("\nStart divisor: %d", total_pixels_sector);
    for (int column = 0; column < SPLITS; column++)
    {
        for (int row = 0; row < SPLITS; row++)
        {
            for (int pixelValue = 0; pixelValue < 256; pixelValue++)
            {
                printf("\nStart divisor: %d", total_pixels_sector);
                for (int tempValue = 0; tempValue < pixelValue; tempValue++)
                {
                    cumulative[row][column][pixelValue] += (double)histograms[row][column][tempValue];
                }
                cumulative[row][column][pixelValue] += (double)histograms[row][column][pixelValue];
                printf("\n cum value:%d, divisor:%d", cumulative[row][column][pixelValue], total_pixels_sector);
                cumulative[row][column][pixelValue] = cumulative[row][column][pixelValue] / (float)36100;
                printf("\ncumulative value. row:%d,column:%d,value:%d", row, column, cumulative[row][column][pixelValue]);
            }
        }
    }

    printf("\nCalculated cumulative");
    // calculate variance for each sector
    for (int row = 0; row < SPLITS; row++)
    {
        for (int column = 0; column < SPLITS; column++)
        {

            for (int pixelValue = 0; pixelValue < 256; pixelValue++)
            {
                double p0 = cumulative[row][column][pixelValue];
                double p1 = 1 - p0;
                double m0 = 0;
                double m1 = 0;

                for (int i = 0; i < pixelValue; i++)
                {
                    m0 += i * histograms[row][column][i] / ((p0 == 0 ? 1 : p0) * total_pixels_sector);
                }

                for (int i = pixelValue; i < 256; i++)
                {
                    m1 += i * histograms[row][column][i] / ((p1 == 0 ? 1 : p1) * total_pixels_sector);
                }
                // update best threshold
                unsigned char tempVar = p0 * p1 * (m0 - m1) * (m0 - m1);
                if (tempVar > varianceHolder[row][column])
                {
                    thresholds[row][column] = pixelValue;
                }
            }
        }
    }
    printf("\ncalculated thresholds");
    // apply thresholds
    for (int row = 0; row < SPLITS; row++)
    {
        for (int column = 0; column < SPLITS; column++)
        {
            for (int x = 0; x < maxDistanceHorz; x++)
            {
                for (int y = 0; y < maxDistanceVert; y++)
                {
                    resultImage[startingX[row] + x][startingY[column] + y] = (inputImage[startingX[row] + x][startingY[column] + y] > thresholds[row][column]) ? 255 : 0;
                }
            }
        }
    }
    printf("\n total_pixels_sector %d", total_pixels_sector);
    printf("\nFinshed otsu");
}
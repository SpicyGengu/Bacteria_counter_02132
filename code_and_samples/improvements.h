#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
// #include <unistd.h>
#include <math.h>

#define detectionSizeIMP 13 // CHANGE THIS ONE. HAS TO BE ODD NUMBER. Må åbenbart ikke være større end 15
unsigned const maxTravelImp = (detectionSizeIMP / 2) - 0.5;

int myMinIMP(int x, int y)
{
    return x < y ? x : y;
}

int applyOtsu(unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    int histogram[256] = {0};
    float cumulative[256] = {0.0f};
    float var[256] = {0.0f};
    int totalPixels = BMP_WIDTH * BMP_HEIGTH;

    // Histogram
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            histogram[image[x][y]]++;
        }
    }

    int cumulativeSum = 0;
    for (int i = 0; i < 256; i++)
    {
        cumulativeSum += histogram[i];
        cumulative[i] = cumulativeSum / (float)totalPixels;
    }

    float maxVariance = 0;
    int bestThreshold = 0;

    for (int i = 0; i < 256; i++)
    {
        int sumClass1 = 0;
        int sumClass2 = 0;
        float meanClass1 = 0.0f;
        float meanClass2 = 0.0f;

        // Class 1 background
        for (int j = 0; j <= i; j++)
        {
            sumClass1 += j * histogram[j];
        }
        meanClass1 = sumClass1 / (cumulative[i] * totalPixels);

        // Class 2 foreground
        for (int j = i + 1; j < 256; j++)
        {
            sumClass2 += j * histogram[j];
        }
        meanClass2 = sumClass2 / ((1 - cumulative[i]) * totalPixels);

        // Variance
        var[i] = cumulative[i] * (1 - cumulative[i]) * (meanClass1 - meanClass2) * (meanClass1 - meanClass2);

        if (var[i] > maxVariance)
        {
            maxVariance = var[i];
            bestThreshold = i;
        }
    }
    return round(bestThreshold);
}

// This is the relevant custom threshold
void betterCustomThreshold(unsigned char input_gs_image[BMP_WIDTH][BMP_HEIGTH], float otsuHold)
{

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            input_gs_image[x][y] = (input_gs_image[x][y] <= otsuHold) ? 0 : 255;
        }
    }
}

char detectHelperWithTolerence(int centerX, int centerY, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    // CHECKS 15x15 area
    int withinExclusion = 0;
    int zDistX = myMinIMP(maxTravelImp, centerX);
    int zDistY = myMinIMP(maxTravelImp, centerY);
    int eDistX = myMinIMP((BMP_WIDTH - 1) - centerX, maxTravelImp);
    int eDistY = myMinIMP((BMP_HEIGTH - 1) - centerY, maxTravelImp);

    // Check border
    if (zDistY == maxTravelImp) // Top
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX - 1; dhx++)
        {
            if (image[dhx][centerY - zDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                withinExclusion++;
            }
        }
    }

    if (zDistX == maxTravelImp) // Left
    {
        for (int dhy = centerY - zDistY + 1; dhy <= centerY + eDistY - 1; dhy++)
        {
            if (image[centerX - zDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                withinExclusion++;
            }
        }
    }

    if (eDistY == maxTravelImp) // Bottom
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX - 1; dhx++)
        {
            if (image[dhx][centerY + eDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                withinExclusion++;
            }
        }
    }

    if (eDistX == maxTravelImp) // Right
    {
        for (int dhy = centerY - zDistY; dhy <= centerY + eDistY; dhy++)
        {
            if (image[centerX + eDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                withinExclusion++;
            }
        }
    }

    if (withinExclusion > 2)
    {
        return 0;
    }

    zDistX = myMinIMP((maxTravelImp - 1), centerX);
    zDistY = myMinIMP((maxTravelImp - 1), centerY);
    eDistX = myMinIMP((BMP_WIDTH - 1) - centerX, (maxTravelImp - 1));
    eDistY = myMinIMP((BMP_HEIGTH - 1) - centerY, (maxTravelImp - 1));

    for (int i = centerX - zDistX; i < centerX + eDistX; i++)
    {
        for (int j = centerY - zDistY; j < centerY + eDistY; j++)
        {
            if (image[i][j])
            {
                return 1;
            }
        }
    }

    return 0;
}

void makeCrossForImprovement(int x, int y, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][3])
{
    int zDistX = myMinIMP(maxTravelImp + 1, x);
    int zDistY = myMinIMP(maxTravelImp + 1, y);
    int eDistX = myMinIMP((BMP_WIDTH - 1) - x, maxTravelImp + 1);
    int eDistY = myMinIMP((BMP_HEIGTH - 1) - y, maxTravelImp + 1);
    for (int i = -1; i < 2; i++)
    {
        for (int xline = x - zDistX; xline <= x + eDistX; xline++)
        {
            input_image[xline][y + i][0] = 255;
            input_image[xline][y + i][1] = 0;
            input_image[xline][y + i][2] = 0;
        }

        for (int yline = y - zDistY; yline <= y + eDistY; yline++)
        {
            input_image[x + i][yline][0] = 255;
            input_image[x + i][yline][1] = 0;
            input_image[x + i][yline][2] = 0;
        }
    }
}

void overWriteForImprovement(int x, int y, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    int zDistX = myMinIMP(maxTravelImp + 1, x);
    int zDistY = myMinIMP(maxTravelImp + 1, y);
    int eDistX = myMinIMP((BMP_WIDTH - 1) - x, maxTravelImp + 1);
    int eDistY = myMinIMP((BMP_HEIGTH - 1) - y, maxTravelImp + 1);
    for (int i = x - zDistX; i <= x + eDistX; i++)
    {
        for (int j = y - zDistY; j <= y + eDistY; j++)
        {
            image[i][j] = 0;
        }
    }
}

void detectImprovement(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH], unsigned char image[BMP_WIDTH][BMP_HEIGTH][3])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (eroded_image[x][y] && detectHelperWithTolerence(x, y, eroded_image))
            {
                // printf("Found a cell at %d %d\n", x, y);
                makeCrossForImprovement(x, y, image);
                totalCount++;
                overWriteForImprovement(x, y, eroded_image);
            }
        }
    }
}
char erodeImprovement(unsigned char img_1[BMP_WIDTH][BMP_HEIGTH], unsigned char img_2[BMP_WIDTH][BMP_HEIGTH], int iterations)
{
    char waseroded = 0;
    if (iterations % 2 == 0)
    {
        for (int x = 0; x < BMP_WIDTH; x++)
        {
            for (int y = 0; y < BMP_HEIGTH; y++)
            {
                if (img_1[x][y])
                { // attempt diamond shape erosion, unsuccessul: if (!(bit_image[x - (x == 0 ? 0 : 1)][y] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && bit_image[x][y - (y == 0 ? 0 : 1)] && bit_image[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - ((x < 2) ? 0 : 2)][y] && bit_image[x + ((x > BMP_WIDTH - 2) ? 0 : 2)][y] && bit_image[x][y - ((y < 2) ? 0 : 2)] && bit_image[x][y + ((y > BMP_HEIGTH - 2) ? 0 : 2)] && bit_image[x - (x == 0 ? 0 : 1)][y - (y == 0 ? 0 : 1)] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                    if (!(img_1[x - (x == 0 ? 0 : 1)][y] && img_1[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && img_1[x][y - (y == 0 ? 0 : 1)] && img_1[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                    {
                        img_2[x][y] = 0;
                        waseroded = 1;
                    }
                    else
                    {
                        img_2[x][y] = 255;
                    }
                }
                else
                {
                    img_2[x][y] = 0;
                }
            }
        }
    }else{
        for (int x = 0; x < BMP_WIDTH; x++)
        {
            for (int y = 0; y < BMP_HEIGTH; y++)
            {
                if (img_2[x][y])
                { // attempt diamond shape erosion, unsuccessul: if (!(bit_image[x - (x == 0 ? 0 : 1)][y] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && bit_image[x][y - (y == 0 ? 0 : 1)] && bit_image[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - ((x < 2) ? 0 : 2)][y] && bit_image[x + ((x > BMP_WIDTH - 2) ? 0 : 2)][y] && bit_image[x][y - ((y < 2) ? 0 : 2)] && bit_image[x][y + ((y > BMP_HEIGTH - 2) ? 0 : 2)] && bit_image[x - (x == 0 ? 0 : 1)][y - (y == 0 ? 0 : 1)] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                    if (!(img_2[x - (x == 0 ? 0 : 1)][y] && img_2[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && img_2[x][y - (y == 0 ? 0 : 1)] && img_2[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                    {
                        img_1[x][y] = 0;
                        waseroded = 1;
                    }
                    else
                    {
                        img_1[x][y] = 255;
                    }
                }
                else
                {
                    img_1[x][y] = 0;
                }
            }
        }
    }
    return waseroded;
}
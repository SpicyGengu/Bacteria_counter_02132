#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
//#include <unistd.h>
#include <math.h>
#include "helpers.h"

#define kernelSize 15
#define imageSize 17
const float sigma = 20;
const float negVal = -1;
const int sigmaTune = 4;
float lowest = 0;
float largest = 0;

void detectBoundary(float image[BMP_WIDTH][BMP_HEIGTH])
{
    unsigned char borderimage[BMP_WIDTH][BMP_HEIGTH];
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            borderimage[x][y] = 0;
        }
    }
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (image[x][y])
            {
                unsigned char neighbours[8] = {image[x - (x - 1 < 0 ? 0 : 1)][y - (y - 1 < 0 ? 0 : 1)], image[x][y - (y - 1 < 0 ? 0 : 1)], image[x + (x + 1 > BMP_HEIGTH ? 0 : 1)][y - (y - 1 < 0 ? 0 : 1)],
                                               image[x - (x - 1 < 0 ? 0 : 1)][y], image[x + (x + 1 > BMP_HEIGTH ? 0 : 1)][y],
                                               image[x - (x - 1 < 0 ? 0 : 1)][y + (y + 1 > BMP_HEIGTH ? 0 : 1)], image[x][y + (y + 1 > BMP_HEIGTH ? 0 : 1)], image[x + (x + 1 > BMP_HEIGTH ? 0 : 1)][y + (y + 1 > BMP_HEIGTH ? 0 : 1)]};
                for (int iterator = 0; iterator < 8; iterator++)
                {
                    if (!neighbours[iterator])
                    {
                        borderimage[x][y] = 255;
                        break;
                    }
                }
            }
        }
    }
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            image[x][y] = borderimage[x][y];
        }
    }
}

void createKernel(float kernel[imageSize][imageSize])
{
    int buffer = (imageSize - kernelSize)/2;
    float maxVal = 0.0f;
    float minVal = 0.0f;

    // Negative Background
    for (int x = 0; x < imageSize; x++) {
        for (int y = 0; y < imageSize; y++) {
            kernel[x][y] = negVal;
        }
    }

    for (int x = 0; x < kernelSize; x++)
    {
        for (int y = 0; y < kernelSize; y++)
        {
            float xOffset = ((kernelSize - 1)/2.0f)-x;
            float yOffset = ((kernelSize - 1)/2.0f)-y;

            float dist = sqrt(xOffset * xOffset + yOffset * yOffset);

            if (dist >= kernelSize / 2.0f){
                continue;
            }

            float currentVal = exp(-dist * dist / (2*(sigma / sigmaTune)* (sigma/sigmaTune)));
            kernel[x + buffer][y+buffer] = currentVal;
        }
    }
}

int isInBounds(int x, int y)
{
    return x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH;
}

void convolve(float input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char charImg[BMP_WIDTH][BMP_HEIGTH], float kernel[kernelSize][kernelSize], int size)
{
    int half_size = size / 2;

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            input_image[x][y] = 0;
        }
    }

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            float sum = 0.0;
            for (int i = -half_size; i <= half_size; i++)
            {
                for (int j = -half_size; j <= half_size; j++)
                {
                    int neighborX = x + i;
                    int neighborY = y + j;
                    if (isInBounds(neighborX, neighborY))
                    {
                        sum += charImg[neighborX][neighborY] * kernel[i + half_size][j + half_size];
                    }
                }
            }
            input_image[x][y] = sum;
        }
    }
}

void customBitThreshold(float input_gs_image[BMP_WIDTH][BMP_HEIGTH], int threshold)
{
    unsigned char processimage[BMP_WIDTH][BMP_HEIGTH];
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            processimage[x][y] = (input_gs_image[x][y] <= threshold) ? 0 : 255;
        }
    }
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            input_gs_image[x][y] = processimage[x][y];
        }
    }
}


char containsWhite(float image[BMP_WIDTH][BMP_HEIGTH])
{
    char containsWhite = 0;
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (image[x][y])
            {
                return 1;
            }
        }
    }
    return 0;
}



void normalizeImage(float image[BMP_WIDTH][BMP_HEIGTH] ){
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            largest = image[x][y] < largest ? image[x][y] : largest;
        }
    }

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            image[x][y] = 255 * (image[x][y] - lowest)/(largest-lowest);
        }
    }
}

void runConvolsionStep(float floatImg[BMP_WIDTH][BMP_HEIGTH], unsigned char charImg[BMP_WIDTH][BMP_HEIGTH], float kernel[kernelSize][kernelSize]){
    convolve(floatImg,charImg,kernel,kernelSize);
    normalizeImage(floatImg);
    floatToCharImg(floatImg,charImg);
}
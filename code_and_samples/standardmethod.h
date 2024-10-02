#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
// #include <unistd.h>
#include <math.h>


unsigned const threshold = 90;
unsigned int totalCount = 0;
#define detectionSize 15 // CHANGE THIS ONE. HAS TO BE ODD NUMBER
unsigned const maxTravel = (detectionSize / 2) - 0.5;

int myMin(int x, int y)
{
    return x < y ? x : y;
}


// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            for (int c = 0; c < BMP_CHANNELS; c++)
            {
                output_image[x][y][c] = 255 - input_image[x][y][c];
            }
        }
    }
}

void greyScale2d(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            greyscale_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
        }
    }
}

void bitThreshold(unsigned char input_gs_image[BMP_WIDTH][BMP_HEIGTH])
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

void applyChannels(unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            for (int c = 0; c < BMP_CHANNELS; c++)
            {
                output_image[x][y][c] = greyscale_image[x][y];
            }
        }
    }
}

char erode(unsigned char bit_image[BMP_WIDTH][BMP_HEIGTH])
{
    unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];
    char waseroded = 0;
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (bit_image[x][y])
            { // attempt diamond shape erosion, unsuccessul: if (!(bit_image[x - (x == 0 ? 0 : 1)][y] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && bit_image[x][y - (y == 0 ? 0 : 1)] && bit_image[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - ((x < 2) ? 0 : 2)][y] && bit_image[x + ((x > BMP_WIDTH - 2) ? 0 : 2)][y] && bit_image[x][y - ((y < 2) ? 0 : 2)] && bit_image[x][y + ((y > BMP_HEIGTH - 2) ? 0 : 2)] && bit_image[x - (x == 0 ? 0 : 1)][y - (y == 0 ? 0 : 1)] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)] && bit_image[x - (x == 0 ? 0 : 1)][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                if (!(bit_image[x - (x == 0 ? 0 : 1)][y] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && bit_image[x][y - (y == 0 ? 0 : 1)] && bit_image[x][y + (y == BMP_HEIGTH - 1 ? 0 : 1)]))
                {
                    eroded_image[x][y] = 0;
                    waseroded = 1;
                }
                else
                {
                    eroded_image[x][y] = 255;
                }
            }
            else
            {
                eroded_image[x][y] = 0;
            }
        }
    }
    // Replaces
    for (int replacex = 0; replacex < BMP_WIDTH; replacex++)
    {
        for (int replacey = 0; replacey < BMP_HEIGTH; replacey++)
        {
            bit_image[replacex][replacey] = eroded_image[replacex][replacey];
        }
    }
    return waseroded;
}

char detectHelper(int centerX, int centerY, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    // CHECKS 15x15 area
    int zDistX = myMin(maxTravel, centerX);
    int zDistY = myMin(maxTravel, centerY);
    int eDistX = myMin((BMP_WIDTH - 1) - centerX, maxTravel);
    int eDistY = myMin((BMP_HEIGTH - 1) - centerY, maxTravel);

    // Check border
    if (zDistY == maxTravel) // Top
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX; dhx++)
        {
            if (image[dhx][centerY - zDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                return 0;
            }
        }
    }

    if (zDistX == maxTravel) // Left
    {
        for (int dhy = centerY - zDistY; dhy <= centerY + eDistY; dhy++)
        {
            if (image[centerX - zDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                return 0;
            }
        }
    }

    if (eDistY == maxTravel) // Bottom
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX; dhx++)
        {
            if (image[dhx][centerY + eDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                return 0;
            }
        }
    }

    if (eDistX == maxTravel) // Right
    {
        for (int dhy = centerY - zDistY; dhy <= centerY + eDistY; dhy++)
        {
            if (image[centerX + eDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                return 0;
            }
        }
    }

    zDistX = myMin((maxTravel - 1), centerX);
    zDistY = myMin((maxTravel - 1), centerY);
    eDistX = myMin((BMP_WIDTH - 1) - centerX, (maxTravel - 1));
    eDistY = myMin((BMP_HEIGTH - 1) - centerY, (maxTravel - 1));

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

void makeCross(int x, int y, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][3])
{
    unsigned char zDistX = myMin(maxTravel + 1, x);
    unsigned char zDistY = myMin(maxTravel + 1, y);
    unsigned char eDistX = myMin((BMP_WIDTH - 1) - x, maxTravel + 1);
    unsigned char eDistY = myMin((BMP_HEIGTH - 1) - y, maxTravel + 1);

    int startX = x - zDistX;
    int endX = x + eDistX;
    int startY = y - zDistY;
    int endY = y + eDistY;

    if (startX < 0) startX = 0;
    if (endX >= BMP_WIDTH) endX = BMP_WIDTH - 1;

    if (startY < 0) startY = 0;
    if (endY >= BMP_HEIGTH) endY = BMP_HEIGTH - 1;

    for (int i = -1; i < 2; i++)
    {
        int yline = y + i;
        if (yline >= 0 && yline < BMP_HEIGTH) {
            for (int xline = startX; xline <= endX; xline++)
            {
                input_image[xline][yline][0] = 255;
                input_image[xline][yline][1] = 0;
                input_image[xline][yline][2] = 0;
            }
        }
    }

    for (int i = -1; i < 2; i++)
    {
        int xline = x + i;
        if (xline >= 0 && xline < BMP_WIDTH) {
            for (int yline = startY; yline <= endY; yline++)
            {
                input_image[xline][yline][0] = 255;
                input_image[xline][yline][1] = 0;
                input_image[xline][yline][2] = 0;
            }
        }
    }
}

void overWrite(int x, int y, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    int zDistX = myMin(maxTravel, x);
    int zDistY = myMin(maxTravel, y);
    int eDistX = myMin((BMP_WIDTH - 1) - x, maxTravel);
    int eDistY = myMin((BMP_HEIGTH - 1) - y, maxTravel);
    for (int i = x - zDistX; i <= x + eDistX; i++)
    {
        for (int j = y - zDistY; j <= y + eDistY; j++)
        {
            image[i][j] = 0;
        }
    }
}

void detect(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH], unsigned char image[BMP_WIDTH][BMP_HEIGTH][3])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (eroded_image[x][y] && detectHelper(x, y, eroded_image))
            {
                // printf("Found a cell at %d %d\n", x, y);
                makeCross(x, y, image);
                totalCount++;
                overWrite(x, y, eroded_image);
            }
        }
    }
}

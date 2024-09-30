#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
//#include <unistd.h>
#include <math.h>


int applyOtsu(unsigned char image[BMP_WIDTH][BMP_HEIGTH]) {
    int histogram[256] = {0};
    float cumulative[256] = {0.0f};
    float var[256] = {0.0f};
    int total_pixels = BMP_WIDTH * BMP_HEIGTH;
    int sum = 0;
    float mean_total = 0.0f;
    
    // Build the histogram and sum of all pixel values
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            int pixel = image[x][y];
            histogram[pixel]++;
            sum += pixel;  // Sum of pixel values
        }
    }

    // Calculate the global mean (mean of all pixel values)
    mean_total = sum / (float)total_pixels;

    // Calculate the cumulative histogram and cumulative mean
    int cumulative_sum = 0;
    for (int i = 0; i < 256; i++) {
        cumulative_sum += histogram[i];
        cumulative[i] = cumulative_sum / (float)total_pixels;
    }

    // Calculate variance for each threshold and find the threshold that maximizes variance
    float max_variance = 0;
    int best_threshold = 0;

    for (int i = 0; i < 256; i++) {
        int sum_class1 = 0;
        int sum_class2 = 0;
        float mean_class1 = 0.0f;
        float mean_class2 = 0.0f;
        
        // Class 1 (background) calculations
        for (int j = 0; j <= i; j++) {
            sum_class1 += j * histogram[j];
        }
        mean_class1 = sum_class1 / (cumulative[i] * total_pixels);

        // Class 2 (foreground) calculations
        for (int j = i + 1; j < 256; j++) {
            sum_class2 += j * histogram[j];
        }
        mean_class2 = sum_class2 / ((1 - cumulative[i]) * total_pixels);

        // Between-class variance
        var[i] = cumulative[i] * (1 - cumulative[i]) * (mean_class1 - mean_class2) * (mean_class1 - mean_class2);

        // Track the largest variance
        if (var[i] > max_variance) {
            max_variance = var[i];
            best_threshold = i;
        }
    }
    return round(best_threshold);
}

void customThreshold(unsigned char input_gs_image[BMP_WIDTH][BMP_HEIGTH], float otsuHold)
{
    unsigned char processimage[BMP_WIDTH][BMP_HEIGTH];
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            processimage[x][y] = (input_gs_image[x][y] <= otsuHold) ? 0 : 255;
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

char detectHelperWithTolerence(int centerX, int centerY, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    // CHECKS 15x15 area
    int withinExclusion = 0;
    int zDistX = min(maxTravel, centerX);
    int zDistY = min(maxTravel, centerY);
    int eDistX = min((BMP_WIDTH - 1) - centerX, maxTravel);
    int eDistY = min((BMP_HEIGTH - 1) - centerY, maxTravel);

    // Check border
    if (zDistY == maxTravel) // Top
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX-1; dhx++)
        {
            if (image[dhx][centerY - zDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                withinExclusion ++;
            }
        }
    }

    if (zDistX == maxTravel) // Left
    {
        for (int dhy = centerY - zDistY; dhy <= centerY + eDistY-1; dhy++)
        {
            if (image[centerX - zDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                withinExclusion ++;
            }
        }
    }

    if (eDistY == maxTravel) // Bottom
    {
        for (int dhx = centerX - zDistX; dhx <= centerX + eDistX-1; dhx++)
        {
            if (image[dhx][centerY + eDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
            {
                withinExclusion ++;
            }
        }
    }

    if (eDistX == maxTravel) // Right
    {
        for (int dhy = centerY - zDistY; dhy <= centerY + eDistY-1; dhy++)
        {
            if (image[centerX + eDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
            {
                withinExclusion ++;
            }
        }
    }

    if (withinExclusion > 3){
        return 0;
    }

    zDistX = min((maxTravel - 1), centerX);
    zDistY = min((maxTravel - 1), centerY);
    eDistX = min((BMP_WIDTH - 1) - centerX, (maxTravel - 1));
    eDistY = min((BMP_HEIGTH - 1) - centerY, (maxTravel - 1));

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
    int zDistX = min(maxTravel + 1, x);
    int zDistY = min(maxTravel + 1 , y);
    int eDistX = min((BMP_WIDTH - 1) - x, maxTravel + 1);
    int eDistY = min((BMP_HEIGTH - 1) - y, maxTravel + 1);
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
    int zDistX = min(maxTravel + 1, x);
    int zDistY = min(maxTravel + 1, y);
    int eDistX = min((BMP_WIDTH - 1) - x, maxTravel + 1);
    int eDistY = min((BMP_HEIGTH - 1) - y, maxTravel + 1);
    for (int i = x - zDistX; i <= x + eDistX; i++)
    {
        for (int j = y - zDistY; j <= y + eDistY; j++)
        {
            image[i][j] = 0;
        }
    }
}

void detectImprovement(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH],unsigned char image[BMP_WIDTH][BMP_HEIGTH][3])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (eroded_image[x][y] && detectHelperWithTolerence(x, y, eroded_image))
            {
                //printf("Found a cell at %d %d\n", x, y);
                makeCrossForImprovement(x, y, image);
                totalCount++;
                overWriteForImprovement(x, y, eroded_image);
            }
        }
    }
}

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
// #include <unistd.h>
#include <math.h>

#define SPLITS 2


int otsuSmall(unsigned char section[BMP_WIDTH / SPLITS][BMP_HEIGTH / SPLITS], int width, int height) {
    int histogram[256] = {0};
    float cumulative[256] = {0.0f};
    float var[256] = {0.0f};
    int totalPixels = width * height;
    int sum = 0;
    float meanTotal = 0.0f;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int pixel = section[x][y];
            histogram[pixel]++;
            sum += pixel;
        }
    }

    meanTotal = sum / (float)totalPixels;

    int cumulativeSum = 0;
    for (int i = 0; i < 256; i++) {
        cumulativeSum += histogram[i];
        cumulative[i] = cumulativeSum / (float)totalPixels;
    }

    float maxVariance = 0;
    int bestThreshold = 0;

    for (int i = 0; i < 256; i++) {
        int sumClass1 = 0;
        int sumClass2 = 0;
        float meanClass1 = 0.0f;
        float meanClass2 = 0.0f;

        for (int j = 0; j <= i; j++) {
            sumClass1 += j * histogram[j];
        }
        if (cumulative[i] > 0) {
            meanClass1 = sumClass1 / (cumulative[i] * totalPixels);
        }

        for (int j = i + 1; j < 256; j++) {
            sumClass2 += j * histogram[j];
        }
        if (1 - cumulative[i] > 0) {
            meanClass2 = sumClass2 / ((1 - cumulative[i]) * totalPixels);
        }

        var[i] = cumulative[i] * (1 - cumulative[i]) * (meanClass1 - meanClass2) * (meanClass1 - meanClass2);

        if (var[i] > maxVariance) {
            maxVariance = var[i];
            bestThreshold = i;
        }
    }
    return bestThreshold;
}

void dynamicOtsu(unsigned char inputImage[BMP_WIDTH][BMP_HEIGTH]) {
    int maxDistanceVert = BMP_HEIGTH / SPLITS;
    int maxDistanceHorz = BMP_WIDTH / SPLITS;

    for (int row = 0; row < SPLITS; row++) {
        for (int column = 0; column < SPLITS; column++) {
            unsigned char section[maxDistanceHorz][maxDistanceVert];

            for (int x = 0; x < maxDistanceHorz; x++) {
                for (int y = 0; y < maxDistanceVert; y++) {
                    section[x][y] = inputImage[row * maxDistanceHorz + x][column * maxDistanceVert + y];
                }
            }

            int threshold = otsuSmall(section, maxDistanceHorz, maxDistanceVert);
            for (int x = 0; x < maxDistanceHorz; x++) {
                for (int y = 0; y < maxDistanceVert; y++) {
                    inputImage[row * maxDistanceHorz + x][column * maxDistanceVert + y] = (inputImage[row * maxDistanceHorz + x][column * maxDistanceVert + y] > threshold) ? 255 : 0;
                }
            }
        }
    }
}

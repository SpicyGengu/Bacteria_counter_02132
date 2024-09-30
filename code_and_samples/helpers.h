//#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
//#include <unistd.h>
#include <math.h>
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


int min(int x, int y)
{
    return x < y ? x : y;
}


int max(int x, int y)
{
    return x > y ? x : y;
}

void floatToCharImg(float floatImg[BMP_WIDTH][BMP_HEIGTH], unsigned char charImg[BMP_WIDTH][BMP_HEIGTH]){
    for (int x = 0; x < BMP_WIDTH; x++){
        for (int y = 0; y < BMP_HEIGTH; y++){
            charImg[x][y] = floatImg[x][y];
        }
    }
}


void charToFloatImg(unsigned char charImg[BMP_WIDTH][BMP_HEIGTH],float floatImg[BMP_WIDTH][BMP_HEIGTH]){
    for (int x = 0; x < BMP_WIDTH; x++){
        for (int y = 0; y < BMP_HEIGTH; y++){
            charImg[x][y] = floatImg[x][y];
        }
    }
}

int averageResult(int results[10]){
    int sumOfResults = 0;
    for (int i = 0; i < 10; i++)
    {
        sumOfResults += results[i];
    }
    return round(sumOfResults/10);
}

int averageResultSize5(int results[5]){
    int sumOfResults = 0;
    for (int i = 0; i < 5; i++)
    {
        sumOfResults += results[i];
    }
    return round(sumOfResults/5);
}
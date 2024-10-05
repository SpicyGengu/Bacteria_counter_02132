#include "standardmethod.h"
#include "improvements.h"
#include "dynamicOtsu.h"
#include <time.h>
int runBaseline(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) // Basic algorithm as given by assignment description
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;
    //clock_t start, end,startThresh,endThresh,startGrey,endGrey,startAlgo,endAlgo;
    //startGrey = clock();
    greyScale2d(input_image, imageToProcess);
    //endGrey = clock();
    //double cpu_time_used = ((double)(endGrey - startGrey)) * 1000 / CLOCKS_PER_SEC;
    //printf("\ngreyscaled done in %f ms\n", cpu_time_used);
    
   // startThresh=clock();
    bitThreshold(imageToProcess);
    //endThresh= clock();
    //cpu_time_used = ((double)(endThresh - startThresh)) * 1000 / CLOCKS_PER_SEC;
    //printf("thresholding done in %f ms\n", cpu_time_used);

    //startAlgo=clock();
    while (erode(imageToProcess))
    {
        detect(imageToProcess, input_image);
    }
    // endAlgo = clock();
    //cpu_time_used = ((double)(endAlgo - startAlgo)) * 1000 / CLOCKS_PER_SEC;
    //printf("\nalgorithm done in %f ms\n", cpu_time_used);
    return totalCount;
}

int runImproved(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) // Improved algorithm
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess1[BMP_WIDTH][BMP_HEIGTH];
    unsigned char imageToProcess2[BMP_WIDTH][BMP_HEIGTH];
    unsigned char(*ptr1)[BMP_HEIGTH] = imageToProcess1; 
    unsigned char(*ptr2)[BMP_HEIGTH] = imageToProcess2;
    unsigned char(*temp)[BMP_HEIGTH] = ptr2;
    //clock_t start, end,startOtsu,endOtsu,startGrey,endGrey,startAlgo,endAlgo;
    //startGrey = clock();
    greyScale2d(input_image, imageToProcess1);
    //endGrey = clock();
    //double cpu_time_used = ((double)(endGrey - startGrey)) * 1000 / CLOCKS_PER_SEC;
    //printf("\ngreyscaled done in %f ms\n", cpu_time_used);

    //startOtsu = clock();
    dynamicOtsu(imageToProcess1);
    //endOtsu = clock();
    //cpu_time_used = ((double)(endOtsu - startOtsu)) * 1000 / CLOCKS_PER_SEC;
    //printf("dynamic otsu done in %f ms\n", cpu_time_used);
//
    //startAlgo = clock();
    while (erodeImprovement(ptr1, ptr2))
    {
        detectImprovement(ptr2, input_image);

        // Swap pointers
        temp = ptr2;
        ptr2 = ptr1;
        ptr1 = temp;
    }
    //endAlgo = clock();
    //cpu_time_used = ((double)(endAlgo - startAlgo)) * 1000 / CLOCKS_PER_SEC;
    //printf("\nalgorithm done in %f ms\n", cpu_time_used);
    return totalCount;
}

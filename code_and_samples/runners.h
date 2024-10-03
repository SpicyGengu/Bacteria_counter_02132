#include "standardmethod.h"
#include "improvements.h"
#include "dynamicOtsu.h"

int runBaseline(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) // Basic algorithm as given by assignment description
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;

    greyScale2d(input_image, imageToProcess);

    bitThreshold(imageToProcess);

    while (erode(imageToProcess))
    {
        detect(imageToProcess, input_image);
    }
    return totalCount;
}

int runImproved(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) // Improved algorithm
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess1[BMP_WIDTH][BMP_HEIGTH];
    unsigned char imageToProcess2[BMP_WIDTH][BMP_HEIGTH];
    unsigned char(*ptr1)[BMP_HEIGTH] = imageToProcess1;
    unsigned char(*ptr2)[BMP_HEIGTH] = imageToProcess2;


    greyScale2d(input_image, imageToProcess1);

    dynamicOtsu(imageToProcess1);

    while (erodeImprovement(ptr1, ptr2))
    {
        detectImprovement(ptr2, input_image);

        // Swap pointers
        unsigned char(*temp)[BMP_HEIGTH] = ptr2;
        ptr2 = ptr1;
        ptr1 = temp;
    }

    return totalCount;
}

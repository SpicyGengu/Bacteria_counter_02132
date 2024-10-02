#include "standardmethod.h"
#include "improvements.h"
#include "dynamicOtsu.h"

int runBaseline(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;

    // Run greyscale
    greyScale2d(input_image, imageToProcess);

    bitThreshold(imageToProcess);

    while (erode(imageToProcess))
    {
        detect(imageToProcess, input_image);
    }
    return totalCount;
}

int runImproved(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess1[BMP_WIDTH][BMP_HEIGTH];
    unsigned char imageToProcess2[BMP_WIDTH][BMP_HEIGTH];
    unsigned char(*ptr1)[BMP_WIDTH] = imageToProcess1; // pointer for swapping
    unsigned char(*ptr2)[BMP_WIDTH] = imageToProcess2; // pointer for swapping

    // Run greyscale
    greyScale2d(input_image, imageToProcess1);

    // betterCustomThreshold(imageToProcess,applyOtsu(imageToProcess));

    dynamicOtsu(imageToProcess1);

    while (erodeImprovement(ptr1, ptr2))
    {
        detectImprovement(ptr1, input_image);
        unsigned char(*temp)[BMP_WIDTH] = ptr2;
        ptr2 = ptr1;
        ptr1 = temp;
    }
    return totalCount;
}
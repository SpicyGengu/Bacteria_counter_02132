#include "standardmethod.h"
#include "improvements.h"

int runBaseline(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;

    // Run greyscale
    greyScale2d(input_image, imageToProcess);

    betterBitThreshold(imageToProcess);

    while (erode(imageToProcess))
    {
        detect(imageToProcess, input_image);
    }
    return totalCount;
}

int runImproved(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    unsigned int maxTravel = round(detectionSize / 2);
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;

    // Run greyscale
    greyScale2d(input_image, imageToProcess);

    betterCustomThreshold(imageToProcess,applyOtsu(imageToProcess));

    while (erode(imageToProcess))
    {
        detectImprovement(imageToProcess, input_image);
    }
    return totalCount;
}
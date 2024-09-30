#include "convulsionMethod.h"
#include "standardmethod.h"
#include "improvements.h"

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
    unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];
    int count = 0;

    // Run greyscale
    greyScale2d(input_image, imageToProcess);

    customThreshold(imageToProcess,applyOtsu(imageToProcess));

    while (erode(imageToProcess))
    {
        detectImprovement(imageToProcess, input_image);
    }
    return totalCount;
}
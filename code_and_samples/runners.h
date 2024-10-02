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
    int iteration = 0;

    // Run greyscale
    greyScale2d(input_image, imageToProcess1);

    //betterCustomThreshold(imageToProcess,applyOtsu(imageToProcess));

    dynamicOtsu(imageToProcess1);

    while (erodeImprovement(imageToProcess1,imageToProcess2,iteration))
    {
        if(iteration % 2 == 0){
            detectImprovement(imageToProcess2, input_image);
        }else{
            detectImprovement(imageToProcess1, input_image);
        }
        iteration++;
    }
    return totalCount;
}
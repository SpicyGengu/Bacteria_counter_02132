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
    unsigned char(*ptr1)[BMP_HEIGTH] = imageToProcess1;
    unsigned char(*ptr2)[BMP_HEIGTH] = imageToProcess2;
    int iteration = 0;

    //printf("Starting runImproved function...\n");
    //printf("Max travel distance: %u\n", maxTravel);

    // Run greyscale
    //printf("Converting image to greyscale...\n");
    greyScale2d(input_image, imageToProcess1);
    //printf("Greyscale conversion completed.\n");

    // Applying dynamic Otsu threshold
    //printf("Applying dynamic Otsu thresholding...\n");
    dynamicOtsu(imageToProcess1);
    //printf("Otsu thresholding completed.\n");

    // Main improvement loop
    //printf("Starting improvement loop...\n");
    while (erodeImprovement(ptr1, ptr2))
    {
        //printf("Erosion completed, detecting improvement...\n");
        detectImprovement(ptr2, input_image);
        //printf("Detection completed.\n");

        // Swap pointers
        unsigned char(*temp)[BMP_HEIGTH] = ptr2;
        ptr2 = ptr1;
        ptr1 = temp;

        iteration++;
        //printf("Iteration %d completed.\n", iteration);
    }

    //printf("Improvement loop finished after %d iterations.\n", iteration);
    //printf("Total cells detected: %d\n", totalCount);

    return totalCount;
}

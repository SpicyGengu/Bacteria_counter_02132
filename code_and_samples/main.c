#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
//#include<unistd.h>
#include <math.h>
#include "bottleNeckmethod.h"
#include "standardmethod.h"
#include "improvements.h"


unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH];
float floatImage[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_bit_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

unsigned char kernel[imageSize][imageSize];



// Main function
int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  
  unsigned int maxTravel = round(detectionSize/2);
  unsigned char imageToProcess[BMP_WIDTH][BMP_HEIGTH];

      // Run greyscale
    greyScale2d(input_image, imageToProcess);

    customThreshold(imageToProcess,applyOtsu(imageToProcess));
    
    while(erode(imageToProcess)){
        detectImprovement(imageToProcess,input_image);
    }

  write_bitmap(input_image,argv[2]);
  printf("Done!\n");
  printf("Total Cells: %d\n", totalCount);
  return 0;
}

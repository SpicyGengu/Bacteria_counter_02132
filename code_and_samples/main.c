#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <math.h>
#include "integrationTests.h"

unsigned int integrationTestMode = 0; // Change if you want integration tests

// Main function
int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Custom program - 02132 - A1\n");
  
  if (integrationTestMode)
  {
    standardRuns();
    improvedRuns();
  }else{
    unsigned char input_image[BMP_HEIGTH][BMP_WIDTH][BMP_CHANNELS];
    read_bitmap(argv[1], input_image);
    runImproved(input_image);
    write_bitmap(input_image,argv[2]);
    printf("Total Cells: %d\n", totalCount);
  }

  
  printf("Done!\n");

  return 0;
}

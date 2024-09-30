#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <math.h>
#include "integrationTests.h"

// Main function
int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Custom program - 02132 - A1\n");
  
  // Load image from file
  //read_bitmap(argv[1], input_image);

  standardRuns();

  improvedRuns();

  //write_bitmap(input_image,argv[2]);
  
  printf("Done!\n");
  //printf("Total Cells: %d\n", totalCount);
  return 0;
}

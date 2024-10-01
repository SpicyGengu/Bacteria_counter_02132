#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <math.h>
#include "integrationTests.h"
#include <time.h>
  
  unsigned char input_image[BMP_HEIGTH][BMP_WIDTH][BMP_CHANNELS];

// Main function
int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Custom program - 02132 - A1\n");
  


  read_bitmap(argv[1], input_image);
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  runImproved(input_image);
  end = clock();
  cpu_time_used = end - start;
  write_bitmap(input_image,argv[2]);
  printf("Done in %f ms!\n",cpu_time_used* 1000 / CLOCKS_PER_SEC);
  printf("Total Cells: %d \n", totalCount);

  start = clock();
  standardRuns();
  end = clock();
  cpu_time_used = end - start;
  printf("Baseline Integration tests done in %f ms!\n",cpu_time_used* 1000 / CLOCKS_PER_SEC);
  start = clock();
  improvedRuns();
  end = clock();
  cpu_time_used = end - start;
  printf("Improved Integration tests done in %f ms!\n",cpu_time_used* 1000 / CLOCKS_PER_SEC);


  return 0;
}

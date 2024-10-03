#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <math.h>
#include "integrationTests.h"
#include <time.h>

// Main function
int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <input file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("\nCustom program - Cell Detection - 02132 - A1\n");

  unsigned char(*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
  if (input_image == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int ii = 0; ii < BMP_HEIGTH; ii++)
    {
      for (int iii = 0; iii < BMP_CHANNELS; iii++)
      {
        input_image[i][ii][iii] = 0;
      }
    }
  }

  read_bitmap(argv[1], input_image);

  clock_t start, end;
  double cpu_time_used;
  start = clock();
  runImproved(input_image);
  end = clock();
  cpu_time_used = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

  write_bitmap(input_image, argv[2]);

  printf("Algorithm run in %f ms\n", cpu_time_used);
  printf("Total Cells: %d \n\n", totalCount);

  free(input_image);



  /***** Integration Tests Start *****/
  
  start = clock();
  standardRuns();
  end = clock();
  cpu_time_used = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
  printf("Baseline integration tests done in %f ms\n", cpu_time_used);

  start = clock();
  improvedRuns();
  end = clock();
  cpu_time_used = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
  printf("Improved integration tests done in %f ms\n", cpu_time_used);

  /***** Integration Tests End *****/

  return 0;
}

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
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Custom program - 02132 - A1\n");

  unsigned char(*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
  if (input_image == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  else
  {
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
  }

  read_bitmap(argv[1], input_image);
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  runImproved(input_image);
  end = clock();
  cpu_time_used = end - start;
  write_bitmap(input_image, argv[2]);
  printf("Done in %f ms!\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);
  printf("Total Cells: %d \n", totalCount);

  free(input_image);

  /***** Integration Tests Start *****/

  start = clock();
  standardRuns();
  end = clock();
  cpu_time_used = end - start;
  printf("Baseline Integration tests done in %f ms!\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);

  start = clock();
  improvedRuns();
  end = clock();
  cpu_time_used = end - start;
  printf("Improved Integration tests done in %f ms!\n", cpu_time_used * 1000 / CLOCKS_PER_SEC);

  /***** Integration Tests End  *****/
  return 0;
}

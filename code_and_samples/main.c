#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include<unistd.h>

/**** Global Variables ****/
unsigned const threshold = 90;
unsigned int totalCount = 0;
unsigned char visited[BMP_WIDTH][BMP_HEIGTH];  // Visited array


unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_bit_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];


// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}

void greyScale2d(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      greyscale_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}



void bitThreshold(unsigned char input_gs_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_bit_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      output_bit_image[x][y] = (input_gs_image[x][y] <= threshold) ? 0 : 255;
    }
  }
}

void applyChannels(unsigned char greyscale_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = greyscale_image[x][y];
      }
    }
  }
}

char erode(unsigned char bit_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];
  char waseroded = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (bit_image[x][y])
      {
        if (!(bit_image[x - (x == 0 ? 0 : 1)][y] && bit_image[x + (x == BMP_WIDTH - 1 ? 0 : 1)][y] && bit_image[x][y - (y == 0 ? 0 : 1)] && bit_image[x][y + (y == 0 ? 0 : 1)]))
        {
          eroded_image[x][y] = 0;
          waseroded = 1;
        }
        else
        {
          eroded_image[x][y] = 255;
        }
      }
      else
      {
        eroded_image[x][y] = 0;
      }
    }
  }
  // Replaces
  for (int replacex = 0; replacex < BMP_WIDTH; replacex++)
  {
    for (int replacey = 0; replacey < BMP_HEIGTH; replacey++)
    {
      bit_image[replacex][replacey] = eroded_image[replacex][replacey];
    }
  }
  return waseroded;
}

int min(int x, int y)
{
  return x < y ? x : y;
}

int max(int x, int y)
{
  return x > y ? x : y;
}

char detectHelper(int centerX, int centerY, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
  if (visited[centerX][centerY])  // Skip if already visited
    return 0;

  // CHECKS 15x15 area
  int zDistX = min(7, centerX);
  int zDistY = min(7, centerY);
  int eDistX = min((BMP_WIDTH - 1) - centerX, 7);
  int eDistY = min((BMP_HEIGTH - 1) - centerY, 7);

  // Check border
  if (zDistY == 7) // Top
  {
    for (int dhx = centerX - zDistX; dhx <= centerX + eDistX; dhx++)
    {
      if (image[dhx][centerY - zDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
      {
        return 0;
      }
    }
  }

  if (zDistX == 7) // Left
  {
    for (int dhy = centerY - zDistY; dhy <= centerY + eDistY; dhy++)
    {
      if (image[centerX - zDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
      {
        return 0;
      }
    }
  }

  if (eDistY == 7) // Bottom
  {
    for (int dhx = centerX - zDistX; dhx <= centerY + eDistY; dhx++)
    {
      if (image[dhx][centerY + eDistY] && dhx != 0 && dhx != BMP_WIDTH - 1)
      {
        return 0;
      }
    }
  }

  if (eDistX == 7) // Right
  {
    for (int dhy = centerY - zDistY; dhy <= centerY + eDistY; dhy++)
    {
      if (image[centerX + eDistX][dhy] && dhy != 0 && dhy != BMP_WIDTH - 1)
      {
        return 0;
      }
    }
  }

  zDistX = min(6, centerX);
  zDistY = min(6, centerY);
  eDistX = min((BMP_WIDTH - 1) - centerX, 6);
  eDistY = min((BMP_HEIGTH - 1) - centerY, 6);

  for (int i = centerX - zDistX; i < centerX + eDistX; i++)
  {
    for (int j = centerY - zDistY; j < centerY + eDistY; j++)
    {
      if (image[i][j])
      {
        return 1;
      }
    }
  }

  return 0;
}

void makeCross(int x, int y)
{
  input_image[x][y][0] = 255;
  input_image[x][y][1] = 0;
  input_image[x][y][2] = 0;
}

void overWrite(int x, int y, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
  int zDistX = min(7, x);
  int zDistY = min(7, y);
  int eDistX = min((BMP_WIDTH - 1) - x, 7);
  int eDistY = min((BMP_HEIGTH - 1) - y, 7);
  for (int i = x - zDistX; i <= x + eDistX; i++)
  {
    for (int j = y - zDistY; j <= y + eDistY; j++)
    {
      image[i][j] = 0;
      visited[i][j] = 1;  // Mark as visited
    }
  }
}

void detect(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (eroded_image[x][y] && detectHelper(x, y, eroded_image))
      {
        printf("\nFound a cell at %d %d", x, y);
        makeCross(x, y);
        totalCount++;
        overWrite(x, y, eroded_image);
      }
    }
  }
}


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

  // Clear visited array
  for (int x = 0; x < BMP_WIDTH; x++)
    for (int y = 0; y < BMP_HEIGTH; y++)
      visited[x][y] = 0;

  // Run greyscale
  greyScale2d(input_image, greyscale_image);

  // Threshold
  bitThreshold(greyscale_image, output_bit_image);

  while(erode(output_bit_image)){
    detect(output_bit_image);
      // Turn to 3d array
    applyChannels(output_bit_image, output_image);

    // Save image to file
    //write_bitmap(output_image, argv[2]);

    sleep(2);
  }

  write_bitmap(input_image,argv[2]);
  printf("Done!\n");
  printf("Total Cells: %d\n", totalCount);
  return 0;
}

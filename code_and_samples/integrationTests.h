#include "runners.h"
#include <stdio.h>
#include "cbmp.h"

#ifdef _WIN32
#include <windows.h>
#define FILE_PATH_FORMAT_EASY "samples\\easy\\%dEASY.bmp"
#define FILE_PATH_FORMAT_MEDIUM "samples\\medium\\%dMEDIUM.bmp"
#define FILE_PATH_FORMAT_HARD "samples\\hard\\%dHARD.bmp"
#define FILE_PATH_FORMAT_IMPOSSIBLE "samples\\impossible\\%dIMPOSSIBLE.bmp"
#else
#include <unistd.h>
#define FILE_PATH_FORMAT_EASY "samples/easy/%dEASY.bmp"
#define FILE_PATH_FORMAT_MEDIUM "samples/medium/%dMEDIUM.bmp"
#define FILE_PATH_FORMAT_HARD "samples/hard/%dHARD.bmp"
#define FILE_PATH_FORMAT_IMPOSSIBLE "samples/impossible/%dIMPOSSIBLE.bmp"
#endif

int averageResult(int results[10])
{
    int sumOfResults = 0;
    for (int i = 0; i < 10; i++)
    {
        sumOfResults += results[i];
    }
    return round(sumOfResults / 10);
}

int averageResultSize5(int results[5])
{
    int sumOfResults = 0;
    for (int i = 0; i < 5; i++)
    {
        sumOfResults += results[i];
    }
    return round(sumOfResults / 5);
}

void standardRuns()
{
    printf("%s\n", FILE_PATH_FORMAT_EASY);
    printf("%s\n", FILE_PATH_FORMAT_MEDIUM);
    printf("%s\n", FILE_PATH_FORMAT_HARD);
    printf("%s\n", FILE_PATH_FORMAT_IMPOSSIBLE);
    unsigned char(*input_image)[BMP_HEIGTH][BMP_CHANNELS] = malloc(BMP_WIDTH * BMP_HEIGTH * BMP_CHANNELS);
    if (input_image == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return; // Handle the error appropriately
    }else{
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
    char file_path[100];
    int easyRuns[10];
    int mediumRuns[10];
    int hardRuns[10];
    int impossibleRuns[5];

    printf("INTEGRATION TESTS FOR BASELINE METHOD\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Easy Difficulties
    printf("Easy difficulties:       ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_EASY, number);
        printf("Attempting to read: %s\n", file_path);
        if (read_bitmap(file_path, input_image) == 0)
        { // Adjust based on how read_bitmap indicates failure
            fprintf(stderr, "Error reading bitmap file: %s\n", file_path);
            continue; // Skip this iteration
        }
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        easyRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(easyRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Medium Difficulties
    printf("Medium difficulties:     ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_MEDIUM, number);
        printf("Attempting to read: %s\n", file_path);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        mediumRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(mediumRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Hard Difficulties
    printf("Hard difficulties:       ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_HARD, number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        hardRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(hardRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Impossible Difficulties
    printf("Impossible Difficulties: ");
    for (int number = 1; number <= 5; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_IMPOSSIBLE, number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        impossibleRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("                                        Average: %d/300", averageResultSize5(impossibleRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n\n");
    free(input_image);
}

void improvedRuns()
{
    unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
    char file_path[100];
    int easyRuns[10];
    int mediumRuns[10];
    int hardRuns[10];
    int impossibleRuns[5];

    printf("INTEGRATION TESTS FOR IMPROVED METHOD\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Easy Difficulties
    printf("Easy difficulties:       ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_EASY, number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        easyRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(easyRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Medium Difficulties
    printf("Medium difficulties:     ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_MEDIUM, number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        mediumRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(mediumRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Hard Difficulties
    printf("Hard difficulties:       ");
    for (int number = 1; number <= 10; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_HARD, number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        hardRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300", averageResult(hardRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Impossible Difficulties
    printf("Impossible Difficulties: ");
    for (int number = 1; number <= 5; number++)
    {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), FILE_PATH_FORMAT_IMPOSSIBLE, number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        impossibleRuns[number - 1] = result;
        printf("%d/300 ", result);
    }
    printf("                                        Average: %d/300", averageResultSize5(impossibleRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
}

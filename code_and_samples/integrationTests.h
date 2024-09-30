#include "runners.h"

void read_bitmap(char *file_path, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);

void standardRuns() {
    unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
    char file_path[100]; 
    int easyRuns[10];
    int mediumRuns[10];
    int hardRuns[10];
    int impossibleRuns[5];

    printf("INTEGRATION TESTS FOR BASELINE METHOD\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    // Easy Difficulties
    printf("Easy difficulties:       ");
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/easy/%dEASY.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        easyRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(easyRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");


    // Medium Difficulties
    printf("Medium difficulties:     ");
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/medium/%dMEDIUM.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        mediumRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(mediumRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Hard Difficulties
    printf("Hard difficulties:       ");
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/hard/%dHARD.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        hardRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(hardRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Impossible Difficulties
    printf("Impossible Difficulties: ");
    for (int number = 1; number <= 5; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/impossible/%dIMPOSSIBLE.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runBaseline(input_image);
        impossibleRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("                                        Average: %d/300",averageResultSize5(impossibleRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n\n");


}

void improvedRuns() {
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
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/easy/%dEASY.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        easyRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(easyRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");



    // Medium Difficulties
    printf("Medium difficulties:     ");
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/medium/%dMEDIUM.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        mediumRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(mediumRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Hard Difficulties
    printf("Hard difficulties:       ");
    for (int number = 1; number <= 10; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/hard/%dHARD.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        hardRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("Average: %d/300",averageResult(hardRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // Impossible Difficulties
    printf("Impossible Difficulties: ");
    for (int number = 1; number <= 5; number++) {
        totalCount = 0;
        snprintf(file_path, sizeof(file_path), "samples/impossible/%dIMPOSSIBLE.bmp", number);
        read_bitmap(file_path, input_image);
        int result = runImproved(input_image);
        impossibleRuns[number-1] = result;
        printf("%d/300 ", result);
    }
    printf("                                        Average: %d/300",averageResultSize5(impossibleRuns));
    printf("\n");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

}
This folder contains the code for the first assignment of course 02132. 

The folder samples_results contains the results of all pictures tested with our algorithm.
If any changes are made to the code, new result images can be made by replacing 'improvedRuns();' with 'improvedRunsWithImages();' on line 65 of main.c

To run the code in the terminal:

Linux/Mac:
- To compile: gcc cbmp.c main.c -o main.out -std=c99
- To run: ./main.out example.bmp example_inv.bmp

Windows:
- To compile: gcc cbmp.c main.c -o main.exe -std=c99
- To run: .\main.exe example.bmp example_inv.bmp

As in the code supplied for this assignment, 'example.bmp' and the result image can be exhanged for other file paths.

To run the built in integration tests, simply replace the value on line 8 of main.c with any whole integer larger than 0 and smaller than 256
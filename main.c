#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main()
{

    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
    file = openfile("ECG.txt");         // Read Data from Sensor
    int total = 0;
    char* _line = readLine(file,&total);

    for (int var = 0; var < total; ++var) {
        printf("%c",_line[var]);
    }

    printf("\n");

    _line = readLine(file,&total);

    for (int var = 0; var < total; ++var) {
        printf("%c",_line[var]);
    }

    printf("\n");

    lowPassFilter();            // Filter Data
                                
    peakDetection(&qsr_params); // Perform Peak Detection


    return fclose(file);
}

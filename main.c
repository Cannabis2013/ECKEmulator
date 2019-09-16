#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

void readFile(FILE *file)
{
    char ch;
    printf("%d",file->_bufsiz);

    while ((ch = fgetc(file)) != EOF) {
        printf("%c",ch);
    }
    printf("\n");

}

int main()
{

    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");

    readFile(file);

    fclose(file);

    /*

    getNextData(file);         // Read Data from Sensor
                                
    lowPassFilter();            // Filter Data
                                
    peakDetection(&qsr_params); // Perform Peak Detection
    */



	return 0;
}

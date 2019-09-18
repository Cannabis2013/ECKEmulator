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

    int _overhead = 12;
    int* _x_Buffer = NULL;
    int *_y_Buffer = NULL;
    if(!(_y_Buffer = malloc((unsigned) _overhead)) ||
            !(_x_Buffer = malloc((unsigned)_overhead)))
        return 0;

    int x = 0;
    while (_overhead)
    {
        int _total = 1;
        int _y = getNextData(file,&_total);
        if(_total == 0)
            _overhead--;

        printf("%d",_y);
        printf("\n");

        x++;
    }

    lowPassFilter();            // Filter Data
                                
    peakDetection(&qsr_params); // Perform Peak Detection


    return fclose(file);
}

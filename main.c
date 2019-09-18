#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>
#include "arraymanipulator.h"

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main()
{
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
    file = openfile("ECG.txt");         // Read Data from Sensor

    int delay = 31;
    int _overhead = delay;
    int* _t_Buffer = NULL;
    int *_unfilt_Buffer = NULL;
    int *_filt_Buffer = malloc(2);

    if(!(_unfilt_Buffer = malloc((unsigned) _overhead)) ||
            !(_t_Buffer = malloc((unsigned)_overhead)))

        return 0;

    int t = 0;
    while (_overhead >= 0)
    {
        int _y = 0;
        int _total = 1;
        // Initializing buffer array
        if(delay >= 0)
        {
            _t_Buffer[_overhead - delay] = t;
            _unfilt_Buffer[_overhead - delay] = getNextData(file,&_total);

            delay--;
        }
        else
        {
            int _filtered_Val = filter(_unfilt_Buffer,delay,_filt_Buffer,delay);
            printf("d",_filtered_Val);
            printf("\n");
            insert(_filt_Buffer,2,_filtered_Val);

        }

        insert(_t_Buffer,delay,++t);
        insert(_unfilt_Buffer,delay,getNextData(file,&_total));

        if(_total == 0)
            _overhead--;
    }
    peakDetection(&qsr_params); // Perform Peak Detection


    return fclose(file);
}

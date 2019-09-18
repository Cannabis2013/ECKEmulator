#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
    file = openfile("ECG.txt");         // Read Data from Sensor

    int _unfiltered_Buffer_Size = 12;
    int delay = _unfiltered_Buffer_Size;
    int _overhead = delay;
    int *_unfiltered_Buffer = malloc((unsigned) _unfiltered_Buffer_Size*sizeof (int));
    int _filtered_Buffer_Size = 31;
    int *_filtered_Buffer = malloc((unsigned)_filtered_Buffer_Size*sizeof (int));
    int t = 0;

    initializeArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,0);
    initializeArray(_filtered_Buffer,_filtered_Buffer_Size,0);

    while (_overhead >= 0)
    {
        int _line_Size = 1;
        int _filtered_Value = 0;
        // Initializing buffer array
        if(delay > 0)
        {
            appendToArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,getNextData(file,&_line_Size));

            if(t > 5)
            {
                lowPassFilter(_unfiltered_Buffer,_filtered_Buffer,_filtered_Buffer_Size,&_filtered_Value);
                appendToArray(_filtered_Buffer,_filtered_Buffer_Size,_filtered_Value);
                printf("%d",_filtered_Value);
                printf("\n");
            }
            delay--;
        }
        else
        {
            _filtered_Value = filter(_unfiltered_Buffer,_unfiltered_Buffer_Size,_filtered_Buffer,_filtered_Buffer_Size, t);
            printf("%d",_filtered_Value);
            printf("\n");
            appendToArray(_unfiltered_Buffer,_overhead,getNextData(file,&_line_Size));
        }

        if(_line_Size == 0)
            _overhead--;

        t++;
    }
    //peakDetection(&qsr_params); // Perform Peak Detection


    return fclose(file);
}

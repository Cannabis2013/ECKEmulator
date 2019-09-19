#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void printValue(int _val, int mode)
{
    printf("%d",_val);
    if(mode = 1)
        printf("\n");
    if(mode == 2)
        printf(" ");
}

int main()
{
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
    file = openfile("ECG.txt");         // Read Data from Sensor

    int _unfiltered_Buffer_Size = 13;
    int delay = 0;
    int _overhead = 50;
    int *_unfiltered_Buffer = malloc((unsigned) _unfiltered_Buffer_Size*sizeof (int));
    int _LP_Filtered_Buffer_Size = 33;
    int *_LP_Filtered_Buffer = malloc((unsigned)_LP_Filtered_Buffer_Size*sizeof (int));
    int _HP_Filtered_Value = 0;
    int t = 0;

    initializeArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,0);
    initializeArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,0);

    while (_overhead >= 0)
    {
        int _line_Size = 1;
        int _filtered_Value = 0;

        // Initializing buffer array
        appendToArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,getNextData(file,&_line_Size));
        int t = _unfiltered_Buffer[12];
        printf("Input: %d",t);
        printf(" ");
        if(delay > 0)
            delay--;
        else
        {
            _filtered_Value = lowPassFilter(_unfiltered_Buffer,_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size);
            appendToArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,_filtered_Value);
            _filtered_Value = highPassFilter(_LP_Filtered_Buffer,_HP_Filtered_Value);
            _HP_Filtered_Value = _filtered_Value;
        }
        printf("Output: %d",_filtered_Value);
        printf("\n");
        if(_line_Size <= 0)
            _overhead--;
        t++;
    }
    //peakDetection(&qsr_params); // Perform Peak Detection


    return fclose(file);
}

#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file;
    if((file = openfile("ECG.txt")) == NULL)
        return -1;

    int _unfiltered_Buffer_Size = 33;
    int _overhead =76;
    int *_unfiltered_Buffer = malloc((unsigned) _unfiltered_Buffer_Size*sizeof (int));
    int _LP_Filtered_Buffer_Size = 33;
    int *_LP_Filtered_Buffer = malloc((unsigned)_LP_Filtered_Buffer_Size*sizeof (int));
    int _HP_Filtered_Buffer_Size = 5;
    int *_HP_Filtered_Buffer = malloc((unsigned) _HP_Filtered_Buffer_Size*sizeof (int));
    int _SQ_Filtered_Buffer_Size = 30;
    int *_SQ_Filtered_Buffer = malloc((unsigned) _SQ_Filtered_Buffer_Size*sizeof (int));
    int _filtered_Buffer_Size = 73;
    int *_filtered_Buffer = malloc((unsigned) _filtered_Buffer_Size*sizeof (int));
    int t = 0;

    initializeArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,0);
    initializeArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,0);
    initializeArray(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size,0);
    initializeArray(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size,0);

    while (_overhead >= 0)
    {
        int _line_Size = 1;
        int _filtered_Value = 0;

        // Initializing buffer array
        appendToArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,getNextData(file,&_line_Size));

        _filtered_Value = lowPassFilter(_unfiltered_Buffer,_unfiltered_Buffer_Size,_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size);
        appendToArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = highPassFilter(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,_HP_Filtered_Buffer[4]);
        appendToArray(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = derivative(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size);
        _filtered_Value *= _filtered_Value;
        appendToArray(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = _moving_Window_Integrator(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size);
        appendToArray(_filtered_Buffer,_filtered_Buffer_Size,_filtered_Value);
        if(t >= 76)
        {
            printf("Samplepoint: %d",t - 76);
            printf(" ");
            printf("Output: %d",_filtered_Buffer[0]);
            printf("\n");
        }
        if(_line_Size <= 0)
            _overhead--;
        t++;
    }

    return fclose(file);
}

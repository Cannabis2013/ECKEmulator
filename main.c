#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void print_Output(int _sample_Point, int _input,int _delay)
{
    printf("Samplepoint: %d",_sample_Point - _delay);
    printf(" ");
    printf("Output: %d",_input);
    printf("\n");
}

int main()
{
    FILE *file;
    if((file = openfile("ECG.txt")) == NULL)
        return -1;


    /*
     * QRS parameters section
     *  - Initialize QRS struct parameters and arrays
     */

    QRS_params * const _QRS = malloc(sizeof (*_QRS));
    _QRS->_SPKF = 0;
    _QRS->_NPKF = 0;
    _QRS->_RR_Low = 0;
    _QRS->_RR_High = 0;
    _QRS->_RR_Miss = 0;
    _QRS->_THRESHOLD1 = 0;
    _QRS->_THRESHOLD2 = 0;
    _QRS->_last_Peak_Position = -1;
    _QRS->_AVG1_Len = 8;
    _QRS->_AVG2_Len = 8;
    _QRS->_RR_AVG1 = malloc((unsigned)_QRS->_AVG1_Len*sizeof (int));
    _QRS->_RR_AVG2 = malloc((unsigned)_QRS->_AVG2_Len*sizeof (int));
    _QRS->_r_Peaks_Size = 8;
    _QRS->_n_Peaks_Size = 8;
    _QRS->_r_Peaks = malloc((unsigned) _QRS->_r_Peaks_Size*sizeof (*_QRS->_r_Peaks));
    _QRS->_n_Peaks = malloc((unsigned) _QRS->_n_Peaks_Size*sizeof (*_QRS->_n_Peaks));

    initializeArray(_QRS->_RR_AVG1,_QRS->_AVG1_Len,0);
    initializeArray(_QRS->_RR_AVG2,_QRS->_AVG2_Len,0);

    initializeArray(_QRS->_RR_AVG1,_QRS->_AVG1_Len,0);
    initializeArray(_QRS->_RR_AVG2,_QRS->_AVG2_Len,0);

    /*
     * QRS parameters section end
     */

    /*
     * Buffer initialization section
     */

    int _unfiltered_Buffer_Size = 13;
    int *_unfiltered_Buffer = malloc((unsigned) _unfiltered_Buffer_Size*sizeof (int));
    int _LP_Filtered_Buffer_Size = 33;
    int *_LP_Filtered_Buffer = malloc((unsigned)_LP_Filtered_Buffer_Size*sizeof (int));
    int _HP_Filtered_Buffer_Size = 5;
    int *_HP_Filtered_Buffer = malloc((unsigned) _HP_Filtered_Buffer_Size*sizeof (int));
    int _SQ_Filtered_Buffer_Size = 30;
    int *_SQ_Filtered_Buffer = malloc((unsigned) _SQ_Filtered_Buffer_Size*sizeof (int));
    int _delay = _unfiltered_Buffer_Size + _LP_Filtered_Buffer_Size +
            _HP_Filtered_Buffer_Size + _SQ_Filtered_Buffer_Size;
    int _filtered_Buffer_Size = _delay;
    int *_filtered_Buffer = malloc((unsigned) _filtered_Buffer_Size*sizeof (int));
    int _overhead =_delay;
    int _t = 0;

    /*
     * Buffer initialization section end
     */

    /*
     * Peak section
     */


    /*
     * Peak section end
     */
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

        if(_t >= _delay)
            print_Output(_t,_filtered_Buffer[0],_delay);

        if(_line_Size <= 0)
            _overhead--;
        _t++;
    }
    // int R_peaks[];
    // int R_peak_timestamps[];
    // int RP_size = sizeof (R_peaks)/ sizeof (R_reaks[0]);
    // int pulse = findPulse(RP_size, t);
    // for (int i = 0; i < RP_size) {
    // printf("%d ", R_peak_timestamps[i]);
    // printf(R_peaks[i]);
    // print("\n");
    // }
    // printf("Pulse: %d", pulse);
    // printf("\n");
    // printf("Warnings: %d", det er nok under peak detection, at man finder frem til,
    // om patienten skal have en warning);

    return fclose(file);
}

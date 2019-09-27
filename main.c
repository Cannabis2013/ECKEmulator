#include "sensor.h"
#include "filters.h"
#include <time.h>


#define PRINT_OUTPUT

int main()
{
    /*
     * File section:
     *  - Open/create files for output/plots
     *  - Return -1 if fails to open/create
     */

    FILE *_file_Input;
    FILE *_file_Filtered_Output = NULL;
    FILE *_file_Output_Peaks = NULL;
    FILE *_file_Output_Peaks_Searchback = NULL;
    FILE *_file_Output_Threshold1 = NULL;

    if(!(_file_Input = openfile("ECG.txt")) ||
            !(_file_Filtered_Output =fopen("filtered_output.txt","w")) ||
            !(_file_Output_Peaks = fopen("peaks.txt","w")) ||
            !(_file_Output_Peaks_Searchback = fopen("peaks_Searchback.txt","w")) ||
            !(_file_Output_Threshold1 = fopen("threshold1_levels","w")))
        return -1;

    /*
     * Check for OS section
     *  - This code only works in linux/unix compliant systems
     *  - It should be noted that we have experienced issues on OSX related to the outputs
     */

#ifdef _WIN64
    return -1;
#endif

    /*
     * QRS parameters section
     *  (1) Initialize QRS struct parameters and arrays with initial values to avoid undefined behaviour
     */

    QRS_params * const _params = malloc(sizeof (*_params));
    _params->_SPKF = 0;
    _params->_NPKF = 0;
    _params->_RR_Low = 0;
    _params->_RR_High = 0;
    _params->_RR_Miss = 0;
    _params->_THRESHOLD1 = 0;
    _params->_THRESHOLD2 = 0;
    _params->_last_Peak_Position = 0;
    _params->_AVG1_Len = 8;
    _params->_AVG2_Len = 8;
    _params->_current_Average = 0;
    _params->_r_Peaks_Size = 0;
    _params->_n_Peaks_Size = 0;
    _params->_prone_For_Warning = 0;

    if(!(_params->_r_Peaks = malloc((unsigned) _params->_r_Peaks_Size*sizeof (*_params->_r_Peaks))) ||
            !(_params->_n_Peaks = malloc((unsigned) _params->_n_Peaks_Size*sizeof (*_params->_n_Peaks))) ||
            !(_params->_RR_AVG1 = malloc((unsigned)_params->_AVG1_Len*sizeof (int))) ||
            !(_params->_RR_AVG2 = malloc((unsigned)_params->_AVG2_Len*sizeof (int))))
    {
        /*
         * Not enough memmory
         */

        return -1;
    }

    initializeArray(_params->_RR_AVG1,_params->_AVG1_Len,-1);
    initializeArray(_params->_RR_AVG2,_params->_AVG2_Len,-1);

    /*
     * Buffer initialization section
     *      (1) Allocate memmory and initialize size variables
     *      (2) Initialize buffer arrays with initial values to avoid undefined behaviour
     */

    int _unfiltered_Buffer_Size = 13;
    int *_unfiltered_Buffer = NULL;
    int _LP_Filtered_Buffer_Size = 33;
    int *_LP_Filtered_Buffer = NULL;
    int _HP_Filtered_Buffer_Size = 5;
    int *_HP_Filtered_Buffer = NULL;
    int _SQ_Filtered_Buffer_Size = 30;
    int *_SQ_Filtered_Buffer = NULL;

    int _delay = _unfiltered_Buffer_Size + _LP_Filtered_Buffer_Size +
            _HP_Filtered_Buffer_Size + _SQ_Filtered_Buffer_Size;

    int _filtered_Buffer_Size = _delay;
    int *_filtered_Buffer = NULL;

    if(!(_unfiltered_Buffer = malloc((unsigned) _unfiltered_Buffer_Size*sizeof (int))) ||
            !(_LP_Filtered_Buffer = malloc((unsigned)_LP_Filtered_Buffer_Size*sizeof (int))) ||
            !(_HP_Filtered_Buffer = malloc((unsigned) _HP_Filtered_Buffer_Size*sizeof (int))) ||
            !(_SQ_Filtered_Buffer = malloc((unsigned) _SQ_Filtered_Buffer_Size*sizeof (int))) ||
            !(_filtered_Buffer = malloc((unsigned) _filtered_Buffer_Size*sizeof (int))))
    {
        /*
         *  Not enough memmory
         */

        return -1;
    }

    initializeArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,0);
    initializeArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,0);
    initializeArray(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size,0);
    initializeArray(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size,0);

    /*
     * Sample section and peak section
     */

    int _overhead =_delay;
    int _sample_Point = 0;
    int _sample_Rate = 250;
    int _current_R_Peak_Index = 0;

    while (_overhead >= 0)
    {
        int _line_Size = 1;
        int _filtered_Value = 0;

        appendToArray(_unfiltered_Buffer,_unfiltered_Buffer_Size,getNextData(_file_Input,&_line_Size));

        _filtered_Value = lowPassFilter(_unfiltered_Buffer,_unfiltered_Buffer_Size,_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size);
        appendToArray(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = highPassFilter(_LP_Filtered_Buffer,_LP_Filtered_Buffer_Size,_HP_Filtered_Buffer[4]);
        appendToArray(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = derivative(_HP_Filtered_Buffer,_HP_Filtered_Buffer_Size);
        _filtered_Value *= _filtered_Value;
        appendToArray(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = _moving_Window_Integrator(_SQ_Filtered_Buffer,_SQ_Filtered_Buffer_Size);
        appendToArray(_filtered_Buffer,_filtered_Buffer_Size,_filtered_Value);

        if(_sample_Point >= _delay)
        {
            int _time_Stamp = (_sample_Point - _delay)*1000/_sample_Rate;

            if(peakDetection(_params,_filtered_Buffer,_time_Stamp))
            {
                if(_current_R_Peak_Index < _params->_r_Peaks_Size)
                {
                    while (_current_R_Peak_Index < _params->_r_Peaks_Size) {
                        Peak _p = _params->_r_Peaks[_current_R_Peak_Index++];
                        int _peak_Time_Stamp = _p._time;
                        int _peak_Value = _p._value;

                        int BPM = 60000/_params->_RR_AVG2[_params->_AVG2_Len - 1];

                        printf("Time: %d Peak value: %d BPM: %d \n" ,
                               _peak_Time_Stamp,_peak_Value,BPM);

                        fprintf(_file_Output_Peaks,"%d; %d \n" ,
                               _peak_Time_Stamp,_peak_Value);


                        if(_p._found_By_Searchback)
                        {
                            fprintf(_file_Output_Peaks_Searchback,"%d; %d \n" ,
                                   _peak_Time_Stamp,_peak_Value);
                        }

                        if(_peak_Value < 2000)
                            printf("WARNING:\n Low heartpeak detected at time: %d",_time_Stamp);
                    }
                }
                if(_params->_prone_For_Warning > 4)
                {
                    printf("Warning:\nIrregularities detected at time: %d\n", _time_Stamp);
                }
            }
            fprintf(_file_Filtered_Output," %d;%d\n",_time_Stamp,_filtered_Value);

            int _threshold1_Value = _params->_THRESHOLD1;
            fprintf(_file_Output_Threshold1,"%d;%d\n",_time_Stamp,_threshold1_Value);
        }
        if(_line_Size <= 0)
            _overhead--;
        _sample_Point++;
    }

    /*
     * Close the files
     */

    fclose(_file_Input);
    fclose(_file_Filtered_Output);
    fclose(_file_Output_Peaks);
    fclose(_file_Output_Peaks_Searchback);
    fclose(_file_Output_Threshold1);

    return 0;
}

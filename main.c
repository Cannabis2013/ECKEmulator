#include "filters.h"
#include <time.h>

#define PRINT_SESSION
#define PRINT_WARNINGS

int main()
{
#ifdef TEST_SESSION
    clock_t _total_Runtime = clock();
    clock_t _average_Calc_Runtime = 0;
    clock_t _average_Contribution = 0;
    clock_t _N = 0;

#endif

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
            !(_file_Output_Threshold1 = fopen("threshold1_levels.txt","w")))
        return -1;

    /*
     * Check for OS section
     *  - This code only works in linux/unix compliant systems
     *  - It should be noted that we have experienced issues on OSX related to the outputs
     */

    fprintf(_file_Output_Peaks,"Time,Value\n");
    fprintf(_file_Filtered_Output,"Time,Value\n");
    fprintf(_file_Output_Threshold1,"Time,Value\n");
    fprintf(_file_Output_Peaks_Searchback,"Time,Value\n");

#ifdef _WIN64
    return -1;
#endif

    /*
     * QRS parameters section
     *  (1) Initialize QRS struct parameters and arrays with initial values to avoid undefined behaviour
     */

    QRS_params * const _params = malloc(sizeof (*_params));

    if(_initialize_QRS_Parameters(_params) == -1)
        return -1;

    /*
     * Buffer initialization section
     *      (1) Allocate memmory and initialize size variables
     *      (2) Initialize buffer arrays with initial values to avoid undefined behaviour
     */

    Buffer_Array *_buffers = malloc(sizeof (*_buffers));
    if(!_initialize_Buffers(_buffers))
        return -1;

    /*
     * Sample section and peak section
     */

    int _overhead =_buffers->_delay;
    int _sample_Point = 0;
    int _current_R_Peak_Index = 0;
    int _sample_Rate = 250;

    while (_overhead >= 0)
    {
        int _line_Size = 1;
        int _filtered_Value = 0;

        appendToArray(_buffers->_unfiltered_Buffer,
                      _buffers->_unfiltered_Buffer_Size,
                      getNextData(_file_Input,&_line_Size));

        _filtered_Value = lowPassFilter(_buffers);
        appendToArray(_buffers->_LP_Filtered_Buffer,_buffers->_LP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = highPassFilter(_buffers,_buffers->_HP_Filtered_Buffer[4]);
        appendToArray(_buffers->_HP_Filtered_Buffer,_buffers->_HP_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = derivative(_buffers);
        _filtered_Value *= _filtered_Value;
        appendToArray(_buffers->_SQ_Filtered_Buffer,_buffers->_SQ_Filtered_Buffer_Size,_filtered_Value);
        _filtered_Value = _moving_Window_Integrator(_buffers);
        appendToArray(_buffers->_filtered_Buffer,_buffers->_filtered_Buffer_Size,_filtered_Value);

        if(_sample_Point >= _buffers->_delay)
        {
            int _time_Points = (_sample_Point - _buffers->_delay)*1000/_sample_Rate;
#ifdef TEST_SESSION
            _average_Contribution = clock();
#endif
            if(peakDetection(_params,_buffers->_filtered_Buffer,_time_Points))
            {
                if(_current_R_Peak_Index < _params->_r_Peaks_Size)
                {
                    while (_current_R_Peak_Index < _params->_r_Peaks_Size) {
                        Peak _p = _params->_r_Peaks[_current_R_Peak_Index++];
                        int _peak_Time_Stamp = _p._time;
                        int _peak_Value = _p._value;

                        int BPM = 60000/(_params->_RR_AVG1[_params->_AVG1_Len - 1]);
#ifdef PRINT_SESSION
                        printf("\nTime: %d Peak value: %d BPM: %d" ,
                               _peak_Time_Stamp,_peak_Value,BPM);
#endif
                        fprintf(_file_Output_Peaks,"%d, %d \n" ,
                               _peak_Time_Stamp,_peak_Value);


                        if(_p._found_By_Searchback)
                        {
                            fprintf(_file_Output_Peaks_Searchback,"%d, %d \n" ,
                                   _peak_Time_Stamp,_peak_Value);
                        }
#ifdef PRINT_SESSION
#ifdef PRINT_WARNINGS
                        if(_peak_Value < 2000)
                            printf(" - WARNING: Low heartpeak detected at time: %d",_p._time);
                        if(_params->_prone_For_Warning > 4)
                            printf(" - Warning: Irregularities detected at time: %d", _p._time);
#endif
#endif
                    }
#ifdef TEST_SESSION
                _average_Calc_Runtime += clock() - _average_Contribution;
                _average_Contribution = 0;
                _N = _N + 1;
#endif
                }
#ifdef TEST_SESSION
                _average_Contribution = 0;
#endif

#ifdef PRINT_SESSION
#endif
            }

            fprintf(_file_Filtered_Output," %d,%d\n",_time_Points -4*2,_buffers->_filtered_Buffer[0]);
            int _threshold1_Value = _params->_THRESHOLD1;
            fprintf(_file_Output_Threshold1,"%d,%d\n",_time_Points - 4*2,_threshold1_Value);
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
#ifdef TEST_SESSION

    _average_Calc_Runtime = _average_Calc_Runtime/_N;

    FILE *_file_Test_Results = NULL;
    if(!(_file_Test_Results = fopen("test_results.txt","w")))
        return -1;
#ifdef PRINT_SESSION
    printf("\nTest session results:\nTotal run-time:    2%d clock cycles\n\n",(int)_total_Runtime);
#endif
    fprintf(_file_Test_Results,"Test session results:\nTotal CPU cycles run-time: %d clock cycles\n",(int)_total_Runtime);
    fprintf(_file_Test_Results,"Average CPU cycles per peak detection: %d clock cycles\n",(int)_average_Calc_Runtime);
    fclose(_file_Test_Results);
#endif

    return 0;
}

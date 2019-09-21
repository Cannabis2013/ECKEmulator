#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{
   int _SPKF;
   int _NPKF;
   int _THRESHOLD1;
   int _THRESHOLD2;
   int _RR_Low;
   int _RR_High;
   int _RR_Miss;
} QRS_params;

typedef struct Peak
{
    /*
     * Noise-peak : _mode = 0
     *     R-peak : _mode = 1
     */

    int _mode;
    int _value;
    int _time;
}Peak;

// Feel free to change and add methods
void peakDetection(QRS_params *params, int _current_Input, int *_last_Input, int *_control_Value);

#endif // QSR_H

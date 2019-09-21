#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{
   int SPKF;
   int NPKF; 
   int THRESHOLD1;
   int THRESHOLD2;
   
} QRS_params;

// Feel free to change and add methods
void peakDetection(QRS_params *params, int _current_Input, int *_last_Input, int *_control_Value);

#endif // QSR_H

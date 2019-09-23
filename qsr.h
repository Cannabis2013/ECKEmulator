#ifndef QSR_H
#define QSR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "arraymanipulator.h"

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct Peak
{
    int _value;
    int _time;
}Peak;

typedef struct QRS_params
{
   int _SPKF;
   int _NPKF;
   int _THRESHOLD1;
   int _THRESHOLD2;
   int _RR_Low;
   int _RR_High;
   int _RR_Miss;
   int _AVG1_Len;
   int _AVG2_Len;
   int *_RR_AVG1;
   int *_RR_AVG2;
   int _last_Peak_Position;

   int _r_Peaks_Size;
   int _n_Peaks_Size;
   Peak *_r_Peaks;
   Peak *_n_Peaks;
} QRS_params;


Peak* _expand_Array(Peak* _peaks, int *_peaks_Size, Peak _p);
bool _searchback_Operation(QRS_params *_params);
void initialize_Peaks(Peak* _peaks,int _peaks_Size);
void appendPeak(Peak *_peaks, int _peaks_Size, Peak _new_Peak);
void _initialize_Parameters(QRS_params *_params, Peak _p, bool _is_Searchback);

bool peakDetection(QRS_params *_params, int *_buffer, int _time_Stamp,int *_regular);

int findPulse(int RR_size, int time);

#endif // QSR_H

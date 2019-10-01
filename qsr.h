#ifndef QSR_H
#define QSR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "arraymanipulator.h"

/*
 * Peak structure
 *  - Peak found by searchback : _found_By_Searchback = 1
 *  - Peak not found by searchback : _found_By_Searchback = 0
 */

typedef struct Peak
{
    int _value;
    int _time;
    int _found_By_Searchback;
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
   int _current_Average;
   int _prone_For_Warning;

   int _r_Peaks_Size;
   int _n_Peaks_Size;
   Peak *_r_Peaks;
   Peak *_n_Peaks;
} QRS_params;

int _initialize_QRS_Parameters(QRS_params *_params);
void _expand_Array(int mode, int *_peaks_Size, Peak _p, QRS_params *_params);
Peak _searchback_Operation(QRS_params *_params);
void initialize_Peaks(Peak* _peaks,int _peaks_Size);
void appendPeak(Peak *_peaks, int _peaks_Size, Peak _new_Peak);

void _initialize_Parameters_R(QRS_params *_params, Peak _p, bool _is_Searchback);
void _initialize_Parameters_Noise(QRS_params *_params, Peak _p);
bool peakDetection(QRS_params *_params, const int * _buffer, int _sample_Point);

#endif // QSR_H

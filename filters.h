#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "arraymanipulator.h"

int lowPassFilter(const int *_input_Buffer, int _input_Buffer_Size, const int *_filtered, int _filtered_Size);
int highPassFilter(const int *_input_Buffer, int _input_Buffer_Size, int _filtered_Point);
int derivative(const int *_input_Buffer, int _input_Buffer_Size);
int _moving_Window_Integrator(const int *_input_Buffer, int _input_Buffer_Size);
 
#endif // FILTERS_H

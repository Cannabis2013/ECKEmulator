#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "arraymanipulator.h"

int lowPassFilter(int *_input_Buffer, int _input_Buffer_Size, int *_filtered, int _filtered_Size);
int highPassFilter(int *_input_Buffer, int _input_Buffer_Size, int _filtered_Point);
int derivative(int* _input_Buffer,int _input_Buffer_Size);
int _moving_Window_Integrator(int* _input_Buffer, int _input_Buffer_Size);
 
#endif // FILTERS_H

#ifndef FILTERS_H
#define FILTERS_H

#include "sensor.h"

int lowPassFilter(Buffer_Array *_buffers);
int highPassFilter(Buffer_Array *_buffers, int _filtered_Point);
int derivative(Buffer_Array *_buffers);
int _moving_Window_Integrator(Buffer_Array *_buffers);
 
#endif // FILTERS_H

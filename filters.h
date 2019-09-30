#ifndef FILTERS_H
#define FILTERS_H

#include "sensor.h"

int lowPassFilter(BUFFER_Arrays * const _buffers);
int highPassFilter(BUFFER_Arrays * const _buffers, int _filtered_Point);
int derivative(BUFFER_Arrays * const _buffers);
int _moving_Window_Integrator(BUFFER_Arrays * const _buffers);
 
#endif // FILTERS_H

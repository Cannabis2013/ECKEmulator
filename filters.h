#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "arraymanipulator.h"

int filter(int *_unfiltered, int _delay, int* _filtered, int filteredSize, int _t);

void lowPassFilter(int *_unfiltered, int *_filtered, int _filtered_Size, int *result);

 
#endif // FILTERS_H

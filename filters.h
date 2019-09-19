#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "arraymanipulator.h"

int lowPassFilter(int *_unfiltered, int *_filtered, int _filtered_Size);
int highPassFilter(int *_unfiltered, int _HP_Filtered_Value);
 
#endif // FILTERS_H

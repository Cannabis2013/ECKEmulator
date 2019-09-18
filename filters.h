#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>

int filter(int *_unfiltered, int _unfiltered_Size, int* _filtered, int filteredSize);

void lowPassFilter(int *_unfiltered, int _unfilteredSize, int *_filtered, int _filtered_Size, int *result);

 
#endif // FILTERS_H

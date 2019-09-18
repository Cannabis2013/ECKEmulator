#include "filters.h"

// LP : y_n = 2y_{n-1} - y_{n-2} + \frac{1}{32}(x_n - 2x_{n-6} + x_{n-12})

int filter(int *_unfiltered, int _delay, int *_filtered, int filteredSize,int _t)
{    
    int _filtered_Value = 0;
    lowPassFilter(_unfiltered,_filtered,filteredSize,&_filtered_Value);
    appendToArray(_filtered,filteredSize,_filtered_Value);
    return _filtered_Value;
}

void lowPassFilter(int *_unfiltered,
                   int* _filtered, int _filtered_Size,int *result)
{
    int _filtered_Sum = 2*_filtered[_filtered_Size - 2] - _filtered[_filtered_Size - 3];
    int a = _unfiltered[12], b = _unfiltered[6], c = _unfiltered[0];
    int _unfiltered_Sum = a - 2*b + c;

    *result = _filtered_Sum + _unfiltered_Sum/32;
}



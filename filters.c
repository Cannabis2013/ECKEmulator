#include "filters.h"

int filter(int *_unfiltered, int _unfiltered_Size, int *_filtered, int filteredSize)
{
    // y_n = 2y_{n-1} - y_{n-2} + \frac{1}{32}(x_n - 2x_{n-6} + x_{n-12})
    int _filtered_Value = 0;
    lowPassFilter(_unfiltered,_unfiltered_Size,_filtered,filteredSize,&_filtered_Value);

    return _filtered_Value;
}

void lowPassFilter(int *_unfiltered, int _unfilteredSize,
                   int* _filtered, int _filtered_Size,int *result)
{
    int _filtered_Sum = 2*_filtered[_filtered_Size - 1] - _filtered[_filtered_Size - 2];
    int _unfiltered_Sum = _unfiltered[_unfilteredSize - 1]
            - _unfiltered[_unfilteredSize - 7] + _unfiltered[0];

    int _filtered_Value = _filtered_Sum + _unfiltered_Sum/32;
}

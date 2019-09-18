#include "filters.h"

int filter(int *_unfiltered, int _delay, int *_filtered, int filteredSize,int _t)
{
    // y_n = 2y_{n-1} - y_{n-2} + \frac{1}{32}(x_n - 2x_{n-6} + x_{n-12})
    int _filtered_Value = 0;
    lowPassFilter(_unfiltered,_delay,_filtered,filteredSize,&_filtered_Value);
    insert(_filtered,filteredSize,_filtered_Value);
    if(_t > 31)
    {
        // TODO: Implement HP filter
    }
    return _filtered_Value;
}

void lowPassFilter(int *_unfiltered, int _unfilteredSize,
                   int* _filtered, int _filtered_Size,int *result)
{
    int _filtered_Sum = 2*_filtered[_filtered_Size - 2] - _filtered[_filtered_Size - 3];
    int _unfiltered_Sum = _unfiltered[11]
            - _unfiltered[5] + _unfiltered[0];

    *result = _filtered_Sum + _unfiltered_Sum/32;
}

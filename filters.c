#include "filters.h"

// LP : y_n = 2y_{n-1} - y_{n-2} + \frac{1}{32}(x_n - 2x_{n-6} + x_{n-12})

int lowPassFilter(int *_unfiltered,int* _filtered, int _filtered_Size)
{
    int _filtered_Sum = 2*_filtered[_filtered_Size - 2] - _filtered[_filtered_Size - 3];
    int _unfiltered_Sum = _unfiltered[12] - 2*_unfiltered[6] + _unfiltered[0];

    return _filtered_Sum + _unfiltered_Sum/32;
}



int highPassFilter(int *_unfiltered, int _HP_Filtered_Value)
{
    return _HP_Filtered_Value - _unfiltered[32]/32 + _unfiltered[16] - _unfiltered[15] + _unfiltered[0]/32;
}

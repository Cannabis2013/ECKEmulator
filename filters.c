#include "filters.h"

int lowPassFilter(const int *_input_Buffer,int _input_Buffer_Size,const int* _filtered, int _filtered_Size)
{
    int _filtered_Dif = 2*_filtered[_filtered_Size - 1]- _filtered[_filtered_Size - 2];
    int _unfiltered_Sum = _input_Buffer[_input_Buffer_Size - 1] -
            2*_input_Buffer[_input_Buffer_Size - 7] + _input_Buffer[_input_Buffer_Size - 13];
    return _filtered_Dif + _unfiltered_Sum/32;
}

int highPassFilter(const int *_input_Buffer, int _input_Buffer_Size,int _filtered_Point)
{
    return _filtered_Point - _input_Buffer[_input_Buffer_Size - 1]/32
            + _input_Buffer[_input_Buffer_Size - 17] -
            _input_Buffer[_input_Buffer_Size - 18] +
            _input_Buffer[_input_Buffer_Size - 33]/32;
}

int derivative(const int *_input_Buffer, int _input_Buffer_Size)
{
    return (2*_input_Buffer[_input_Buffer_Size - 1] + _input_Buffer[_input_Buffer_Size - 2]
            - _input_Buffer[_input_Buffer_Size -4] - 2*_input_Buffer[_input_Buffer_Size -5])/8;
}

int _moving_Window_Integrator(const int *_input_Buffer, int _input_Buffer_Size)
{
    int N = 30;
    int sum = 0;
    for (int var = 1; var <= N; ++var)
        sum += _input_Buffer[(_input_Buffer_Size -1) - (N - var)];

    return sum/N;
}

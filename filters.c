#include "filters.h"

// LP : y_n = 2y_{n-1} - y_{n-2} + \frac{1}{32}(x_n - 2x_{n-6} + x_{n-12})
// HP : y_n = y_{n-1} - \frac{x_n}{32} + x_{n-16} - x_{n-17} + x_{n-32}

int lowPassFilter(int *_input_Buffer,int _input_Buffer_Size,int* _filtered, int _filtered_Size)
{
    int _filtered_Dif = 2*_filtered[_filtered_Size - 1]- _filtered[_filtered_Size - 2];
    int _unfiltered_Sum = _input_Buffer[_input_Buffer_Size - 1] -
            2*_input_Buffer[_input_Buffer_Size - 7] + _input_Buffer[_input_Buffer_Size - 13];
    return _filtered_Dif + _unfiltered_Sum/32;
}

int highPassFilter(int *_input_Buffer, int _input_Buffer_Size,int _filtered_Point)
{
    return _filtered_Point - _input_Buffer[_input_Buffer_Size - 1]/32
            + _input_Buffer[_input_Buffer_Size - 17] -
            _input_Buffer[_input_Buffer_Size - 18] +
            _input_Buffer[_input_Buffer_Size - 33]/32;
}

int derivative(int *_input_Buffer, int _input_Buffer_Size)
{
    return (1/8)*(_input_Buffer[_input_Buffer_Size - 1] + _input_Buffer[_input_Buffer_Size - 2]
            - _input_Buffer[_input_Buffer_Size -4] - 2*_input_Buffer[_input_Buffer_Size -5]);
}

int _moving_Window_Integrator(int *_input_Buffer, int _input_Buffer_Size)
{
    int N = 30;
    int sum = 0;
    for (int var = 1; var <= N; ++var)
        sum += _input_Buffer[(_input_Buffer_Size -1) - (N - var)];

    return (1/N)*sum;
}

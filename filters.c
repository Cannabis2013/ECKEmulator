#include "filters.h"

int lowPassFilter(Buffer_Array *_buffers)
{
    int _filtered_Size = _buffers->_LP_Filtered_Buffer_Size;
    int *_filtered = _buffers->_LP_Filtered_Buffer;
    int _input_Buffer_Size = _buffers->_unfiltered_Buffer_Size;
    int *_input_Buffer = _buffers->_unfiltered_Buffer;

    int _filtered_Dif = 2*_filtered[_filtered_Size - 1]- _filtered[_filtered_Size - 2];
    int _unfiltered_Sum = _input_Buffer[_input_Buffer_Size - 1] -
            2*_input_Buffer[_input_Buffer_Size - 7] + _input_Buffer[_input_Buffer_Size - 13];

    return _filtered_Dif + _unfiltered_Sum/32;
}

int highPassFilter(Buffer_Array *_buffers,int _filtered_Point)
{
    int _input_Buffer_Size = _buffers->_LP_Filtered_Buffer_Size;
    int *_input_Buffer = _buffers->_LP_Filtered_Buffer;

    return _filtered_Point - _input_Buffer[_input_Buffer_Size - 1]/32
            + _input_Buffer[_input_Buffer_Size - 17] -
            _input_Buffer[_input_Buffer_Size - 18] +
            _input_Buffer[_input_Buffer_Size - 33]/32;
}

int derivative(Buffer_Array *_buffers)
{
    int _input_Buffer_Size = _buffers->_HP_Filtered_Buffer_Size;
    int *_input_Buffer = _buffers->_HP_Filtered_Buffer;

    return (2*_input_Buffer[_input_Buffer_Size - 1] + _input_Buffer[_input_Buffer_Size - 2]
            - _input_Buffer[_input_Buffer_Size -4] - 2*_input_Buffer[_input_Buffer_Size -5])/8;
}

int _moving_Window_Integrator(Buffer_Array *_buffers)
{
    int _input_Buffer_Size = _buffers->_SQ_Filtered_Buffer_Size;
    int *_input_Buffer = _buffers->_SQ_Filtered_Buffer;

    int N = 30;
    int sum = 0;
    for (int var = 1; var <= N; ++var)
        sum += _input_Buffer[(_input_Buffer_Size -1) - (N - var)];

    return sum/N;
}

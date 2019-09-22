#include "arraymanipulator.h"

void appendToArray(int *_seq, int total, int _value)
{
    for (int var = 0; var < total - 1; ++var)
    {
        int val = _seq[var + 1];
        _seq[var] = val;
    }

    _seq[total - 1] = _value;
}

void initializeArray(int *_seq, int total, int _initial_Value)
{
    for (int var = 0; var < total; ++var)
        _seq[var] = _initial_Value;
}

int average(int *_input, int _input_Lenght,int _N)
{
    int sum = 0, _index_Of_last_Element = _input_Lenght - 1;

    for (int var = 0; var < _N; ++var)
       sum += _input[_index_Of_last_Element - var];

    return sum/_N;
}

#include "arraymanipulator.h"

void appendToArray(int *_seq, int total, int _value)
{
    for (int var = 0; var < total - 1; ++var)
        _seq[var] = _seq[var + 1];

    _seq[total - 1] = _value;
}

void initializeArray(int *_input, int _input_Lenght, int _initial_Value)
{
    if(_input == NULL)
        _Exit(-1);

    for (int var = 0; var < _input_Lenght; ++var)
        _input[var] = _initial_Value;
}

int average(const int * _input, int _input_Lenght,int _N)
{
    if(_input == NULL)
        _Exit(-1);

    int sum = 0;
    int _index_Of_last_Element = _input_Lenght - 1;
    int _n = 0;
    for (int var = _index_Of_last_Element; var >= _input_Lenght - _N ; --var)
    {
        if(_input[var] == -1)
            break;

        sum += _input[var];
        _n++;
    }

    return sum/_n;
}

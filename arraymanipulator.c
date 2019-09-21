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



int average(int *_seq, int total)
{
    int N = 0, sum = 0;
    for (int var = total - 1; var > 0; --var) {
       int val = _seq[var];
       if(val == -1)
           return sum/N;
       sum += val;
       N++;
    }

    return sum;
}

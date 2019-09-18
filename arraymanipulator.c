#include "arraymanipulator.h"

void insert(int *_Seq, int total, int _value)
{
    for (int var = 0; var < total - 1; ++var)
        _Seq[var] = _Seq[var + 1];

    _Seq[total - 1] = _value;
}

#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int* getNextData(FILE *_file, int _lookAhead,int *total)
{
    *total = 0;
    int* result = malloc((unsigned int)_lookAhead*sizeof (int));
    for (int var = 0; var < _lookAhead; ++var) {
        int _lenght = 0;
        char* _line = readLine(_file,&_lenght);
        if(_lenght != 0)
        {
            result[var] = toInteger(_line,_lenght);
            *total = *total + 1;
        }
    }

    if(*total < _lookAhead)
        return malloc((unsigned int)*total * sizeof (int));

    return result;
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r");

   return file;
}

char *readLine(FILE *_file,int* _lineSize)
{
    fpos_t _cPos = 0;
    fgetpos(_file,&_cPos);
    int _buffSize = 0;
    int condition = 1;
    while(condition)
    {
        char c = (char) fgetc(_file);
        if(c == EOF || (int) c == 10)
            condition = 0;
        else
            _buffSize++;
    }

    fsetpos(_file,&_cPos);

    char* _line =  malloc((unsigned int) _buffSize);
    int _index = 0;
    while (_index < _buffSize)
        _line[_index++] = (char) fgetc(_file);

    *_lineSize = _buffSize;

    fgetpos(_file,&_cPos);
    _cPos++;
    fsetpos(_file,&_cPos);
    return _line;
}

int toInteger(char *_data, int total)
{
    int multi = 1;
    if(_data[0] == '-')
    {
        _data[0] = 48;
        multi = -1;
    }

    int base = 1;
    int result = 0;
    for (int var = total - 1; var >= 0; --var) {
        int c = _data[var] - 48;
        result += c * base;
        base *= 10;
    }

    return result * multi;
}

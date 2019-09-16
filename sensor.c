#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int* getNextData(FILE *_file,char* _buf, int _lookAhead)
{

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
        char c = fgetc(_file);
        if(c == EOF || (int) c == 10)
            condition = 0;
        else
            _buffSize++;
    }
    fsetpos(_file,&_cPos);

    char* _line = malloc(_buffSize);
    int _index = 0;
    while (_index < _buffSize)
        _line[_index++] = fgetc(_file);

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

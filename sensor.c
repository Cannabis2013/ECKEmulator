#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *_file, int *total)
{
    char *_line = readLine(_file,total);
    int _result = toInteger(_line,*total);

    free(_line);

    return _result;

}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r");

   return file;
}

#ifdef WIN64
char *readLine(FILE *_file,int* _lineSize)
{
    *_lineSize = 0;
    fpos_t _cPos = 0;
    fgetpos(_file,&_cPos);
    int _buffSize = 0;
    int condition = 1;
    while(condition)
    {
        char c = (char) fgetc(_file);
        if(c == EOF || c == 10)
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
    _cPos++;
    fsetpos(_file,&_cPos);
    return _line;
}

#elif linux

char *readLine(FILE *_file,int* _lineSize)
{
    char* _line = NULL;
    getLine(&_line,&_lineSize,_file);

    return _line;
}

#elif unix

char *readLine(FILE *_file,int* _lineSize)
{
    char* _line = NULL;
    getLine(&_line,&_lineSize,_file);

    return _line;
}

#endif

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

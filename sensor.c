#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *_file,char* _buf, int _lookAhead)
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
    char _buff;
    int _index = 0;
    while (_index < _buffSize && (_buff = fgetc(_file)) != EOF)
        _line[_index++] = _buff;

    *_lineSize = _buffSize;

    fgetpos(_file,&_cPos);
    _cPos++;
    fsetpos(_file,&_cPos);
    return _line;
}

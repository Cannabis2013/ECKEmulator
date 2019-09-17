#include "sensor.h"

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

#elif defined (linux)
char *readLine(FILE *_file,int* _lineSize)
{
    char* _line = NULL;
    size_t _line_Buff_Size = 0;
    *_lineSize = (int) getline(&_line,&_line_Buff_Size,_file);

    char* _result = malloc((unsigned int) *_lineSize - 1);
    for (int var = 0; var < *_lineSize - 1; ++var)
       _result[var] = _line[var];
    *_lineSize = *_lineSize - 1;
    free(_line);
    return _result;
}
#elif unix

char *readLine(FILE *_file,int* _lineSize)
{
    char* _line = NULL;
    size_t _line_Buff_Size = 0;
    *_lineSize = (int) getline(&_line,&_line_Buff_Size,_file);

    char* _result = malloc((unsigned int) *_lineSize - 1);
    for (int var = 0; var < *_lineSize - 1; ++var)
       _result[var] = _line[var];
    *_lineSize = *_lineSize - 1;
    free(_line);
    return _result;
}

#endif

#ifdef WIN64
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
#elif defined (linux)
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
#elif defined (unix)
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
#endif


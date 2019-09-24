#include "sensor.h"

int getNextData(FILE *_file, int *total)
{
    char *_file_Line = readLine(_file,total);
    int _result = toInteger(_file_Line,*total);

    free(_file_Line);

    return _result;
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r");

   return file;
}

#if defined (linux)
char *readLine(FILE *_file,int* const _lineSize)
{
    char* _line = NULL;
    size_t _line_Buff_Size = 0;
    *_lineSize = (int) getline(&_line,&_line_Buff_Size,_file);
    char* _result = malloc((unsigned int) *_lineSize - 1);
    for (int var = 0; var < *_lineSize - 1; ++var)
       _result[var] = _line[var];
    *_lineSize =  *_lineSize <= 0 ? 0 : *_lineSize - 1;
    free(_line);
    return _result;
}
#elif defined(__APPLE_CC__)

char *readLine(FILE *_file,int* _lineSize)
{
    char* _line = NULL;
    size_t _line_Buff_Size = 0;
    *_lineSize = (int) getline(&_line,&_line_Buff_Size,_file);

    char* _result = malloc((unsigned int) *_lineSize - 1);
    for (int var = 0; var < *_lineSize - 1; ++var)
       _result[var] = _line[var];
    *_lineSize =  *_lineSize <= 0 ? 0 : *_lineSize - 1;
    free(_line);
    return _result;
}

#endif

#if defined (linux)
int toInteger(char *const _data, int total)
{
    if(total == 0)
        return 0;
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
#elif defined (__APPLE_CC__)
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


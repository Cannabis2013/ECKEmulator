#ifndef SENSOR_H
#define SENSOR_H

#include "qsr.h"

typedef struct Buffer_Array
{
    int _delay;

    int _unfiltered_Buffer_Size;
    int _LP_Filtered_Buffer_Size;
    int _HP_Filtered_Buffer_Size;
    int _SQ_Filtered_Buffer_Size;
    int _filtered_Buffer_Size;

    int *_unfiltered_Buffer;
    int *_LP_Filtered_Buffer;
    int *_HP_Filtered_Buffer;
    int *_SQ_Filtered_Buffer;
    int *_filtered_Buffer;

}Buffer_Array;

FILE* openfile(const char* filename);
int getNextData(FILE *_file, int *total);
int toInteger(char *_data, int total);
char *readLine(FILE *_file, int * _lineSize);
int _initialize_Buffers(Buffer_Array *_buffers);

#endif // SENSOR_H

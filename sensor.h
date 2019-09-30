#ifndef SENSOR_H
#define SENSOR_H

#include "qsr.h"

// Header file for the sensor functionality
// You are free to change and add functions 
// as much as you like

typedef struct BUFFER_Arrays
{
    int _delay;

    int _unfiltered_Buffer_Size;
    int _LP_Filtered_Buffer_Size;
    int _HP_Filtered_Buffer_Size;
    int _SQ_Filtered_Buffer_Size;

    int *_unfiltered_Buffer;
    int *_LP_Filtered_Buffer;
    int *_HP_Filtered_Buffer;
    int *_SQ_Filtered_Buffer;

    int _filtered_Buffer_Size;
    int *_filtered_Buffer;
}BUFFER_Arrays;


FILE* openfile(const char* filename);
int getNextData(FILE *_file, int *total);
int toInteger(char *_data, int total);
char *readLine(FILE *_file, int * _lineSize);
int _initialize_Buffers(BUFFER_Arrays* _buffers);
#endif // SENSOR_H

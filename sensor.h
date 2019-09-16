#ifndef SENSOR_H
#define SENSOR_H
#include <stdio.h>
#include <stdlib.h>

// Header file for the sensor functionality
// You are free to change and add functions 
// as much as you like

FILE* openfile(const char* filename);

int *getNextData(FILE *_file, int _lookAhead, int *total);

int toInteger(char* _data, int total);

char* readLine(FILE *_file, int *_lineSize);

#endif // SENSOR_H

#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file)
{
   return 0; // return sensor value
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r"); // Replace NULL with the actual file object

   return file;
}

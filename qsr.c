#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void peakDetection(QRS_params *params)
{

}

int findPulse(int RP_size, int time) {
    float seconds = time/250;
    float beats_pr_min = (RP_size/seconds)*60;
    return (int)  beats_pr_min;
}

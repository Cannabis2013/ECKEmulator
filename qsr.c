#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void peakDetection(QRS_params *params)
{

}

int findPulse(int RP_size, int time) {
    int milliseconds = time*1000/250;
    int beats_pr_min = (RP_size*1000)/milliseconds*60;
    return beats_pr_min;
}

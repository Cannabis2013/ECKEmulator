#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void peakDetection(QRS_params *params, int _current_Input, int *_last_Input,int* _control_Value)
{
    /*
     * _control_Value = 0 if x_{n-2} < x_{n-1}
     * _control_Value = 1 if x_{n-2} > x_{n-1}
     * _control_Value = 2 if x_{n-2} == x_{n-1}
     * _control_Value = 3 for n == 0
     */

    /*
     * Start conditions:
     *  Assumes x_{-1}=x_0
     */

    if(*_control_Value == 3)
        *_control_Value = 2;
    else
    {
        if(*_control_Value == 0 && _current_Input < *_last_Input)
        {
            // A peak candidate as the preceding value was greather than its predessor
            // TODO: Implement some further peak analasys
            *_control_Value = 0;
        }
        else if(_current_Input < *_last_Input)
            *_control_Value  = 0;
        else if(_current_Input > *_last_Input)
            *_control_Value = 1;
        else
            *_control_Value = 2;
    }

    *_last_Input = _current_Input;
}

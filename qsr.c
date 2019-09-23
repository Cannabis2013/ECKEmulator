#include "qsr.h"

Peak *_expand_Array(Peak *_peaks, int *_peaks_Size, Peak _p)
{
    *_peaks_Size = *_peaks_Size + 1;
    _peaks = realloc(_peaks,(unsigned)*_peaks_Size + 1);
    _peaks[*_peaks_Size + 1] = _p;

    return _peaks;
}

void appendPeak(Peak *_peaks, int _peaks_Size, Peak _new_Peak)
{
    for (int var = 0; var < _peaks_Size - 1; ++var)
    {
        Peak _p = _peaks[var + 1];
        _peaks[var] = _p;
    }

    _peaks[_peaks_Size - 1] = _new_Peak;
}

void initialize_Peaks(Peak *_peaks, int _peaks_Size)
{
    for (int var = 0; var < _peaks_Size; ++var) {
        Peak _p;
        _p._time = -1;
        _p._value = 0;
        _peaks[var] = _p;
    }
}

bool peakDetection(QRS_params *_params, int *_buffer, int _time_Stamp,int *_regular)
{
    int _last_Peak_Position = _params->_last_Peak_Position;
    int _peak_Candidate = _buffer[1], _preceding = _buffer[2], _next = _buffer[0];
    if(_peak_Candidate > _preceding && _peak_Candidate > _next)
    {
<<<<<<< HEAD
        if(_peak_Candidate > _params->_THRESHOLD1)
=======
        Peak _p;
        _p._time = _time_Stamp;
        _p._value = _peak_Candidate;

        _expand_Array(_params->_n_Peaks,&_params->_n_Peaks_Size,_p);
        if(_p._value > _params->_THRESHOLD1)
>>>>>>> exp
        {
            int _current_RR_Interval = _time_Stamp - _last_Peak_Position ;
            /*
             * We have a R-peak
             */

            if((_current_RR_Interval < _params->_RR_Low || _current_RR_Interval > _params->_RR_High)
                    && _last_Peak_Position != -1)
            {
<<<<<<< HEAD
                /*
                 * WARNING:
                 *      Interval falls out of the interval-limit and is prone for a warning.
                 */
=======
>>>>>>> exp
                if(_current_RR_Interval > _params->_RR_Miss)
                {
                    /*
                     * TODO: Perform search back procedure
                     * TODO: Reinitialize the _current_RR_Interval with regard to the new peak found
                     */

                    _searchback_Operation(_params);
                }
                else
<<<<<<< HEAD
                    *_regular = *_regular + 1;
            }

            Peak _p;
            _p._time = _time_Stamp;
            _p._value = _peak_Candidate;
            _expand_Array(_params->_r_Peaks,&_params->_r_Peaks_Size,_p);

            _initialize_Parameters(_params,_p,false);
=======
                {
                    /*
                     * WARNING:
                     *      Interval falls out of the interval-limit and the system is prone for a warning.
                     */

                    *_regular = *_regular + 1;
                }
            }

            _expand_Array(_params->_r_Peaks,&_params->_r_Peaks_Size,_p);
            _initialize_Parameters_Full(_params,_p,false);
>>>>>>> exp

            return true;
        }
        else
        {
            /*
             * A noise peak detected
             */

<<<<<<< HEAD
            int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;

            _params->_NPKF = _peak_Candidate/8 + (7*_NPKF)/8;
            _params->_THRESHOLD1 = _NPKF + (_SPKF - _NPKF)/4;
            _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

            Peak _p;
            _p._time = _time_Stamp;
            _p._value = _peak_Candidate;

            _expand_Array(_params->_n_Peaks,&_params->_n_Peaks_Size,_p);
=======
            _initialize_Parameters_Semi(_params,_p);
>>>>>>> exp
        }
    }

    return false;
}

<<<<<<< HEAD
void _initialize_Parameters(QRS_params *_params, Peak _p, bool _is_Searchback)
=======
void _initialize_Parameters_Full(QRS_params *_params, Peak _p, bool _is_Searchback)
>>>>>>> exp
{
    int _current_RR_Interval = _p._time - _params->_last_Peak_Position;
    int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;
    if(_params->_last_Peak_Position != -1)
    {
        appendToArray(_params->_RR_AVG2,_params->_AVG2_Len,_current_RR_Interval);
        appendToArray(_params->_RR_AVG1,_params->_AVG1_Len,_current_RR_Interval);
    }

    int avg = _is_Searchback ? average(_params->_RR_AVG1,_params->_AVG1_Len,8):
                               average(_params->_RR_AVG2,_params->_AVG2_Len,8);

    _params->_RR_Low = (92*avg)/100;
    _params->_RR_High = (116*avg)/100;
    _params->_RR_Miss = (166*avg)/100;

    _params->_SPKF = _p._value/8 +(7*_SPKF)/8;
    _params->_THRESHOLD1 = _NPKF + (_SPKF - _NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

    _params->_last_Peak_Position = _p._time;
}

bool _searchback_Operation(QRS_params *_params)
{
    int _last_Peak_Position = _params->_last_Peak_Position;
    int _threshold_2 = _params->_THRESHOLD2;
    for (int var = 0; var < _params->_n_Peaks_Size; ++var) {
        Peak _n_Peak = _params->_n_Peaks[var];
        int _peak_Time = _n_Peak._time;
        if(_n_Peak._value > _threshold_2 && _peak_Time >= _last_Peak_Position)
        {
            appendPeak(_params->_r_Peaks,_params->_r_Peaks_Size,_n_Peak);
<<<<<<< HEAD
            _initialize_Parameters(_params,_n_Peak,true);
=======
            _initialize_Parameters_Full(_params,_n_Peak,true);
>>>>>>> exp
            return true;
        }
    }
    return false;
}

<<<<<<< HEAD
int findPulse(int RP_size, int time) {
    int milliseconds = time*1000/250;
    int beats_pr_min = (RP_size*1000)/milliseconds*60;
    return beats_pr_min;
=======
void _initialize_Parameters_Semi(QRS_params *_params, Peak _p)
{
    int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;

    _params->_NPKF = _p._value/8 + (7*_NPKF)/8;
    _params->_THRESHOLD1 = _NPKF + (_SPKF - _NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;
>>>>>>> exp
}

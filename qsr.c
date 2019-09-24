#include "qsr.h"

void _expand_Array(int _mode, int *_peaks_Size, Peak _p,QRS_params *_params)
{
    /*
     * Noise-Peak for _mode = 0
     * R-Peak for _mode = 1
     */

    *_peaks_Size = *_peaks_Size + 1;
    if(_mode == 0)
    {
        _params->_n_Peaks = realloc(_params->_n_Peaks,(unsigned)*_peaks_Size*sizeof (Peak));
        _params->_n_Peaks[*_peaks_Size - 1] = _p;
    }
    else
    {
        _params->_r_Peaks = realloc(_params->_r_Peaks,(unsigned)*_peaks_Size*sizeof (Peak));
        _params->_r_Peaks[*_peaks_Size - 1] = _p;
    }
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
        Peak _p;
        _p._time = _time_Stamp;
        _p._value = _peak_Candidate;

        _expand_Array(0,&_params->_n_Peaks_Size,_p,_params);
        if(_p._value > _params->_THRESHOLD1)
        {
            /*
             * We have a R-peak
             */

            int _current_RR_Interval = (_params->_r_Peaks_Size != 0) ?  _time_Stamp - _last_Peak_Position : 0;

            if((_current_RR_Interval < _params->_RR_Low || _current_RR_Interval > _params->_RR_High))
            {
                if(_current_RR_Interval > _params->_RR_Miss)
                {
                    /*
                     * TODO: Perform search back procedure
                     * TODO: Reinitialize the _current_RR_Interval with regard to the new peak found
                     */

                    return _searchback_Operation(_params);
                }
                else
                {
                    /*
                     * WARNING:
                     *      Interval falls out of the interval-limit and the system is prone for a warning.
                     * TODO:
                     *      Implement some sort of warning feature which notifies the staff about irregular peaks
                     */

                    *_regular = *_regular + 1;
                    return false;
                }
            }
            _expand_Array(1,&_params->_r_Peaks_Size,_p,_params);
            _initialize_Parameters_Full(_params,_p,false);
            return true;
        }
        else
        {
            /*
             * A noise peak detected
             */

            _initialize_Parameters_Semi(_params,_p);
        }
    }

    return false;
}

void _initialize_Parameters_Full(QRS_params *_params, Peak _p, bool _is_Searchback)
{
    int _current_RR_Interval = _p._time - _params->_last_Peak_Position;
    _params->_SPKF = !_is_Searchback ?  _p._value/8 +(7*_params->_SPKF)/8 : _p._value/4 +(3*_params->_SPKF)/4 ;

    if(!_is_Searchback)
        appendToArray(_params->_RR_AVG2,_params->_AVG2_Len,_current_RR_Interval);
    appendToArray(_params->_RR_AVG1,_params->_AVG1_Len,_current_RR_Interval);

    int avg = _is_Searchback ? average(_params->_RR_AVG1,_params->_AVG1_Len,8):
                               average(_params->_RR_AVG2,_params->_AVG2_Len,8);

    _params->_RR_Low = (92*avg)/100;
    _params->_RR_High = (116*avg)/100;
    _params->_RR_Miss = (166*avg)/100;

    _params->_THRESHOLD1 = _params->_NPKF + (_params->_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

    _params->_last_Peak_Position = _p._time;
}

bool _searchback_Operation(QRS_params *_params)
{
    int _last_Peak_Position = _params->_last_Peak_Position;
    int _threshold_2 = _params->_THRESHOLD2;
    for (int var = _params->_n_Peaks_Size - 1; var >= 0; --var) {
        Peak _n_Peak = _params->_n_Peaks[var];
        int _peak_Time = _n_Peak._time;
        if(_n_Peak._value > _threshold_2 && _peak_Time > _last_Peak_Position)
        {
            _expand_Array(1,&_params->_r_Peaks_Size,_n_Peak,_params);

            _initialize_Parameters_Full(_params,_n_Peak,true);
            return true;
        }
    }
    return false;
}


// David Tran
int _find_Pulse(int RP_size, int time) {
    int milliseconds = time*1000/250;
    int beats_pr_min = (RP_size*1000)/milliseconds*60;
    return beats_pr_min;
}
void _initialize_Parameters_Semi(QRS_params *_params, Peak _p)
{
    int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;

    _params->_NPKF = _p._value/8 + (7*_NPKF)/8;
    _params->_THRESHOLD1 = _params->_NPKF + (_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;
}

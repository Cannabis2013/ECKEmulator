#include "qsr.h"

void _expand_Array(int _mode, int *_peaks_Size, Peak _p,QRS_params *_params)
{
    /*
     * Noise-Peak : _mode = 0
     * R-Peak : _mode = 1
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

bool peakDetection(QRS_params *_params, const int * _buffer, int _time_Point)
{
    int _last_Peak_Time = _params->_last_Peak_Position;
    int _current = _buffer[2];
    int _peak_Candidate = _buffer[1];
    int _preceding = _buffer[0];

    int _peak_Time = _time_Point - 4;
    int _current_RR_Interval = _peak_Time - _last_Peak_Time;
    if(_peak_Candidate > _current && _peak_Candidate > _preceding)
    {
        Peak _p;
        _p._value = _peak_Candidate;
        _p._time = _peak_Time;

        _expand_Array(0,&_params->_n_Peaks_Size,_p,_params);
        if(_p._value > _params->_THRESHOLD1)
        {
            /*
             * R-peak detected
             */

            if(_last_Peak_Time <= 0)
                goto jmp;

            if((_current_RR_Interval < _params->_RR_Low || _current_RR_Interval > _params->_RR_High))
            {
                if(_current_RR_Interval > _params->_RR_Miss)
                {
                    /*
                     * TODO: Perform search back procedure
                     * TODO: Reinitialize the _current_RR_Interval accordingly to the new peak found
                     */

                    _p = _searchback_Operation(_params);
                    if(_p._value == -1)
                        return false;
                    _expand_Array(1,&_params->_r_Peaks_Size,_p,_params);
                    _initialize_Parameters_R(_params,_p,true);
                    return true;
                }
                /*
                 * WARNING:
                 *      Interval falls out of the interval-limit and the system is prone for a warning.
                 */
                _params->_prone_For_Warning = _params->_prone_For_Warning + 1;

                return false;
            }
            jmp:;
            _p._found_By_Searchback = 0;
            _expand_Array(1,&_params->_r_Peaks_Size,_p,_params);
            _initialize_Parameters_R(_params,_p,false);
            return true;
        }

        /*
         * A noise peak detected
         */

        _initialize_Parameters_Noise(_params,_p);
    }

    if(_current_RR_Interval > _params->_RR_Miss)
    {
        Peak _p = _searchback_Operation(_params);
        if(_p._value == -1)
            return false;
        int _peak_Interval = _p._time - _params->_last_Peak_Position;
        if(_peak_Interval < _params->_RR_Low)
        {
            _params->_prone_For_Warning = _params->_prone_For_Warning + 1;

            return true;
        }

        _expand_Array(1,&_params->_r_Peaks_Size,_p,_params);
        _initialize_Parameters_R(_params,_p,true);
        return true;
    }

    return false;
}

Peak _searchback_Operation(QRS_params *_params)
{
    int _last_Peak_Position = _params->_last_Peak_Position;
    int _threshold = _params->_THRESHOLD2;

    for (int var = _params->_n_Peaks_Size - 1; var >= 0; --var) {
        Peak _n_Peak = _params->_n_Peaks[var];
        int _peak_Time = _n_Peak._time;
        if(_n_Peak._value > _threshold  && _peak_Time > _last_Peak_Position)
        {
            _n_Peak._found_By_Searchback = 1;
            return _n_Peak;
        }
    }
    Peak _empty_peak;
    _empty_peak._value = -1;
    return _empty_peak;

}

void _initialize_Parameters_R(QRS_params *_params, Peak _p, bool _is_Searchback)
{
    int _current_RR_Interval = _p._time - _params->_last_Peak_Position;
    _params->_SPKF = !_is_Searchback ?  (_p._value + 7*_params->_SPKF)/8 : (_p._value +3*_params->_SPKF)/4 ;

    if(!_is_Searchback)
        appendToArray(_params->_RR_AVG2,_params->_AVG2_Len,_current_RR_Interval);
    appendToArray(_params->_RR_AVG1,_params->_AVG1_Len,_current_RR_Interval);

    int avg = !_is_Searchback ?  average(_params->_RR_AVG2,_params->_AVG2_Len,8) :
                                 average(_params->_RR_AVG1,_params->_AVG1_Len,8);

    _params->_RR_Low = (80*avg)/100;
    _params->_RR_High = (120*avg)/100;
    _params->_RR_Miss = (180*avg)/100;

    _params->_current_Average = avg;

    _params->_THRESHOLD1 = _params->_NPKF + (_params->_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

    _params->_last_Peak_Position = _p._time;

    _params->_prone_For_Warning = 0;
}

void _initialize_Parameters_Noise(QRS_params *_params, Peak _p)
{
    int _NPKF = _params->_NPKF;
    int _SPKF = _params->_SPKF;

    _params->_NPKF = (_p._value + 7*_NPKF)/8;
    _params->_THRESHOLD1 = _params->_NPKF + (_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;
}

int _initialize_QRS_Parameters(QRS_params *_params)
{
    _params->_SPKF = 5000;
    _params->_NPKF = 4950;
    _params->_RR_Low = 92*300/100;
    _params->_RR_High = 116*300/100;
    _params->_RR_Miss = 166*300/100;
    _params->_THRESHOLD1 = 5000;
    _params->_THRESHOLD2 = 2500;
    _params->_last_Peak_Position = 0;
    _params->_AVG1_Len = 8;
    _params->_AVG2_Len = 8;
    _params->_current_Average = 0;
    _params->_r_Peaks_Size = 0;
    _params->_n_Peaks_Size = 0;
    _params->_prone_For_Warning = 0;

    if(!(_params->_r_Peaks = malloc((unsigned) _params->_r_Peaks_Size*sizeof (*_params->_r_Peaks))) ||
            !(_params->_n_Peaks = malloc((unsigned) _params->_n_Peaks_Size*sizeof (*_params->_n_Peaks))) ||
            !(_params->_RR_AVG1 = malloc((unsigned)_params->_AVG1_Len*sizeof (int))) ||
            !(_params->_RR_AVG2 = malloc((unsigned)_params->_AVG2_Len*sizeof (int))))
    {
        /*
         * Not enough memmory
         */

        return -1;
    }

    initializeArray(_params->_RR_AVG1,_params->_AVG1_Len,300);
    initializeArray(_params->_RR_AVG2,_params->_AVG2_Len,300);

    return 0;
}

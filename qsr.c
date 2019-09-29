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

bool peakDetection(QRS_params *_params, const int * _buffer, int _time_Stamp)
{
    int _last_Peak_Position = _params->_last_Peak_Position;
    int _peak_Candidate = _buffer[1];
    int _preceding = _buffer[2];
    int _next = _buffer[0];
    int _current_RR_Interval = (_params->_r_Peaks_Size != 0) ?  _time_Stamp - _last_Peak_Position : 0;

    if(_peak_Candidate > _preceding && _peak_Candidate > _next)
    {
        Peak _p;
        _p._time = _time_Stamp + 4;
        _p._value = _peak_Candidate;

        _expand_Array(0,&_params->_n_Peaks_Size,_p,_params);
        if(_p._value > _params->_THRESHOLD1)
        {
            /*
             * R-peak detected
             */


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

                /*
                 * WARNING:
                 *      Interval falls out of the interval-limit and the system is prone for a warning.
                 */

                _params->_prone_For_Warning = _params->_prone_For_Warning + 1;
                return true;
            }
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
        /*
         * TODO: Perform search back procedure
         * TODO: Reinitialize the _current_RR_Interval with regard to the new peak found
         */

        return _searchback_Operation(_params);
    }

    return false;
}

void _initialize_Parameters_R(QRS_params *_params, Peak _p, bool _is_Searchback)
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

    _params->_current_Average = avg;

    _params->_THRESHOLD1 = _params->_NPKF + (_params->_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

    _params->_last_Peak_Position = _p._time;

    _params->_prone_For_Warning = 0;
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
            _n_Peak._found_By_Searchback = 1;
            _expand_Array(1,&_params->_r_Peaks_Size,_n_Peak,_params);
            _initialize_Parameters_R(_params,_n_Peak,true);
            return true;
        }
    }
    return false;
}

void _initialize_Parameters_Noise(QRS_params *_params, Peak _p)
{
    int _NPKF = _params->_NPKF;
    int _SPKF = _params->_SPKF;

    _params->_NPKF = _p._value/8 + (7*_NPKF)/8;
    _params->_THRESHOLD1 = _params->_NPKF + (_SPKF - _params->_NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;
}

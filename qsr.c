#include "qsr.h"

Peak *expandArray(Peak *_peaks, int *_peaks_Size, Peak _p)
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

bool peakDetection(QRS_params *_params, int *_buffer, int _time_Stamp)
{
    int _last_Peak_Position = _params->_r_Peaks[_params->_r_Peaks_Size - 1]._time;
    int _current_RR_Interval = _time_Stamp - _last_Peak_Position ;
    int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;
    int _peak_Candidate = _buffer[1], _preceding = _buffer[2], _appending = _buffer[0];

    if(_current_RR_Interval > _params->_RR_Miss)
    {
        /*
         * TODO: Perform search back procedure
         * TODO: Reinitialize the _current_RR_Interval with regard to the new peak found
         */

        int _threshold_2 = _params->_THRESHOLD2;

        for (int var = 0; var < _params->_n_Peaks_Size; ++var) {
            Peak _n_Peak = _params->_n_Peaks[var];

            if(_n_Peak._value > _threshold_2)
            {
                appendPeak(_params->_r_Peaks,_params->_r_Peaks_Size,_n_Peak);

                _initialize_Parameters(_params,_n_Peak,false);

                return true;
            }
        }
    }

    if(_peak_Candidate > _preceding && _peak_Candidate > _appending)
    {
        if(_peak_Candidate > _params->_THRESHOLD1)
        {
            /*
             * We have a R-peak
             */

            if((_current_RR_Interval < _params->_RR_Low || _current_RR_Interval > _params->_RR_High)
                    && _last_Peak_Position != -1)
            {
                /*
                 * WARNING:
                 *      Interval falls out of the interval-limit and issues a warning.
                 */
            }

            Peak _p;
            _p._time = _time_Stamp;
            _p._value = _peak_Candidate;
            appendPeak(_params->_r_Peaks,_params->_r_Peaks_Size,_p);

            _initialize_Parameters(_params,_p,false);

            return true;
        }
        else
        {
            /*
             * A noise peak detected
             */

            _params->_NPKF = _peak_Candidate/8 + (7*_NPKF)/8;
            _params->_THRESHOLD1 = _NPKF + (_SPKF - _NPKF)/4;
            _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

            Peak _p;
            _p._time = _time_Stamp;
            _p._value = _peak_Candidate;

            expandArray(_params->_n_Peaks,&_params->_n_Peaks_Size,_p);

        }
    }

    return false;
}

void _initialize_Parameters(QRS_params *_params, Peak _p, bool _is_Searchback)
{
    int _current_RR_Interval = _p._time - _params->_last_Peak_Position;
    int _NPKF = _params->_NPKF, _SPKF = _params->_SPKF;
    if(_params->_last_Peak_Position != -1)
    {
        appendToArray(_params->_RR_AVG2,_params->_AVG2_Len,_current_RR_Interval);
        appendToArray(_params->_RR_AVG1,_params->_AVG1_Len,_current_RR_Interval);
    }
    int avg = 0;
    if(_is_Searchback)
        avg = average(_params->_RR_AVG1,_params->_AVG1_Len,8);
    else
        avg = average(_params->_RR_AVG2,_params->_AVG2_Len,8);

    _params->_RR_Low = (92*avg)/100;
    _params->_RR_High = (116*avg)/100;
    _params->_RR_Miss = (166*avg)/100;

    _params->_SPKF = _p._value/8 +(7*_SPKF)/8;
    _params->_THRESHOLD1 = _NPKF + (_SPKF - _NPKF)/4;
    _params->_THRESHOLD2 = _params->_THRESHOLD1/2;

    _params->_last_Peak_Position = _p._time;
}

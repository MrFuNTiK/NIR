#include <stdexcept>
#include "program_environment.hpp"
#include "../GCC/GCC_class.hpp"
#include "../GPS/GPS_class.hpp"

programm_environment::programm_environment() :
    _window_size(0),
    _sample_rate(0),
    _meth(GPS_TDE),
    _weight_fn(NONE)
{}

programm_environment::~programm_environment()
{}

std::shared_ptr<programm_environment> programm_environment::GetInstance()
{
    if( _pe.get() == nullptr )
    {
        _pe.reset(new programm_environment());
    }
    return _pe;
}

void programm_environment::SetWindowSize(uint16_t window_size)
{
    _window_size = window_size;
}

void programm_environment::SetSampleRate(uint16_t sample_rate)
{
    _sample_rate = sample_rate;
}

void programm_environment::SetMethodTDE(tde_meth meth)
{
    _meth = meth;
}

void programm_environment::SetWeightingFunction(weighting_func weighting_fn)
{
    _weight_fn = weighting_fn;
}

TDE* programm_environment::GetCalculator()
{
    TDE* tde = nullptr;
    if(_sample_rate == 0)
    {
        throw std::runtime_error("Sample rate was not set");
    }
    if(_window_size == 0)
    {
        throw std::runtime_error("Windows size was not set");
    }

    if( _meth == GCC_TDE )
    {
        tde = new GCC(_window_size, _sample_rate, _weight_fn);
    }
    else if( _meth == GPS_TDE )
    {
        tde = new GPS(_window_size, _sample_rate, _weight_fn);
    }
    return tde;
}

std::shared_ptr<programm_environment> programm_environment::_pe(nullptr);

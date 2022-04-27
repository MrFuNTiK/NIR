#include <stdexcept>

#include <program_environment.hpp>
#include <GCC_class.hpp>
#include <GPS_class.hpp>

program_environment::program_environment() :
    _window_size(0),
    _sample_rate(0),
    _window_avrg_num(1),
    _meth(GPS_TDE),
    _weight_fn(NONE)
{}

program_environment::~program_environment()
{}

std::shared_ptr<program_environment> program_environment::GetInstance()
{
    if( _pe.get() == nullptr )
    {
        _pe.reset(new program_environment());
    }
    return _pe;
}

void program_environment::SetWindowSize(uint16_t window_size)
{
    if(window_size < 4)
    {
        throw std::logic_error("Window size must be 4 at least");
    }
    if((window_size & (window_size-1)))
    {
        throw std::logic_error("Window size must be a power of 2");
    }
    _window_size = window_size;
}

void program_environment::SetSampleRate(uint16_t sample_rate)
{
    if(!sample_rate)
    {
        throw std::logic_error("Sampling frequency must be greater than 0");
    }
    _sample_rate = sample_rate;
}

void program_environment::SetWinAvrgNum(uint16_t num)
{
    if(!num)
    {
        throw std::logic_error("Windows avergaging number must be greater than 0");
    }
    _window_avrg_num = num;
}

void program_environment::SetMethodTDE(tde_meth meth)
{
    _meth = meth;
}

void program_environment::SetWeightingFunction(weighting_func weighting_fn)
{
    _weight_fn = weighting_fn;
}

TDE* program_environment::CreateCalculator()
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
    if(NONE != _weight_fn && _window_avrg_num == 1)
    {
        throw std::logic_error("Avergaging number must be greater than 1 to apply frequncy-weighting function");
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

uint16_t program_environment::GetWindowSize()
{
    return _window_size;
}

uint16_t program_environment::GetSampleRate()
{
    return _sample_rate;
}

uint16_t program_environment::GetWinAvrgNum()
{
    return _window_avrg_num;
}

std::shared_ptr<program_environment> program_environment::_pe(nullptr);
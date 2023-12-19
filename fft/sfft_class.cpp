#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <FFT/sfft_class.hpp>
#include <logger/logger.hpp>

using namespace transform::cpu::forward;
using namespace logger;

SFFT::SFFT(size_t _size) :
    size(_size)
{
    if(4 > size)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }
    if(size & (size-1))
    {
        throw std::logic_error("Window size must be a power of 2");
    }
    slidingHandle.reset( SlidingFFT_create( size ) );
    TRACE_EVENT(EVENTS::CREATE, "created");
}

SFFT::~SFFT()
{
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void SFFT::Execute()
{
    Forward::Execute();
}

void SFFT::Execute( const std::vector< double >& real_ )
{
    for( const auto sample : real_ )
    {
        SlidingFFT_update( slidingHandle.get(), sample );
    }
}

void SFFT::Conjugate()
{
    auto spectrum = SlidingFFT_GetSpectrum( slidingHandle.get() );
    for( size_t i = 0; i < size/2+1; ++i )
    {
        spectrum[ i ] = spectrum[ i ] / size;
    }
}

void SFFT::SetReal(const std::vector<double>& real_)
{
    Forward::SetReal(real_);
}

void SFFT::GetFourierImage(std::vector<std::complex<double>>& _fourier) noexcept
{
    auto spectrum = SlidingFFT_GetSpectrum( slidingHandle.get() );
    for( size_t i = 0; i < size/2+1; ++i )
    {
        _fourier[ i ] = spectrum[ i ] / size;
    }
}

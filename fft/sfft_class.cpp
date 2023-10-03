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
    real_array.resize(size);
    fourier_image.resize(size/2+1);
    slidingHandle.reset( SlidingFFT_create( size ) );
    TRACE_EVENT(EVENTS::CREATE, "created");
}

SFFT::~SFFT()
{
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void SFFT::Execute() noexcept
{
    for( const auto sample : real_array )
    {
        SlidingFFT_update( slidingHandle.get(), sample );
    }

    const auto res = SlidingFFT_GetSpectrum( slidingHandle.get() );
    for( size_t i = 0; i < size / 2 + 1; ++i )
    {
        fourier_image[ i ] = res[ i ];
    }

    NormalizeFur();
}

void SFFT::NormalizeFur()
{
    for (size_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i] /= size;
    }
}

void SFFT::Conjugate() noexcept
{
    for (size_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i] = std::conj(fourier_image[i]);
    }
}

void SFFT::SetReal(const std::vector<double>& _real) noexcept
{
    memcpy(&real_array[0], &_real[0], sizeof(double)*size);
}

void SFFT::GetFourierImage(std::vector<std::complex<double>>& _fourier) noexcept
{
    memcpy(&_fourier[0], &fourier_image[0], sizeof(std::complex<double>)*(size/2+1));
}

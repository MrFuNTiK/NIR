#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <FFT/grz_forward_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

using namespace transform::cpu::grz;
using namespace logger;

Forward::Forward(uint16_t _size) :
    size(_size)
{
    if(4 > size)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }

    real_array.resize(size);
    fourier_image.resize(size/2+1);
    goerzHandle = GoerzelTF_create( size );
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class created");
}

Forward::~Forward()
{
    GoerzelTF_destroy( goerzHandle );
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class destroyed");
}

void Forward::SetBounds( size_t lowerBound, size_t upperBound )
{
    if( lowerBound > size/2u+1u || upperBound > size/2u+1u )
    {
        throw std::logic_error( "Bounds must be less then siz+1" );
    }

    if( lowerBound > upperBound )
    {
        throw std::logic_error( "Lower bound is greater then upper" );
    }
    lowerBound_ = lowerBound;
    upperBound_ = upperBound;

    fourier_image.resize( upperBound - lowerBound );
}

void Forward::Execute() noexcept
{
    size_t freqIndex = lowerBound_;
    for( auto& harmonica : fourier_image )
    {
        if( !GoerzelTF_set_freq_idx( goerzHandle, freqIndex ) )
        {
            TRACE_EVENT( EVENTS::ERROR, "GoerzelTF_set_freq_idx() failed" );
        }
        for( auto sample : real_array )
        {
            if( !GoerzelTF_update( goerzHandle, sample ) )
            {
                TRACE_EVENT( EVENTS::ERROR, "GoerzelTF_update() failed" );
            }
        }
        harmonica = GoerzelTF_result( goerzHandle );
        ++freqIndex;
    }
}

void Forward::NormalizeFur()
{
    for (auto& harmonica : fourier_image)
    {
        harmonica /= size;
    }
}

void Forward::Conjugate() noexcept
{
    for (auto& harmonica : fourier_image)
    {
        harmonica = std::conj(harmonica);
    }
}

void Forward::SetReal(const std::vector<double>& _real) noexcept
{
    memcpy(&real_array[0], &_real[0], sizeof(double)*size);
}

void Forward::GetFourierImage(std::vector<std::complex<double>>& _fourier) noexcept
{
    memcpy(&_fourier[0], &fourier_image[0], sizeof(std::complex<double>)*(size/2+1));
}

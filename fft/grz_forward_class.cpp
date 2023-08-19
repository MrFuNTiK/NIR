#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <memory>

#include <FFT/grz_forward_class.hpp>
#include <logger/logger.hpp>
//#include <core.hpp>

using namespace transform::cpu::grz;
using namespace logger;

Forward::Forward(size_t _size, size_t lowerBound, size_t upperBound) :
    size(_size)
{
    if(4 > size)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }

    if( lowerBound > size/2u+1u || upperBound > size/2u+1u )
    {
        throw std::logic_error( "Bounds must be less then size+1" );
    }

    if( lowerBound > upperBound )
    {
        throw std::logic_error( "Lower bound is greater then upper" );
    }
    lowerBound_ = lowerBound;
    upperBound_ = upperBound;
    goerzHandle.reset( GoerzelTF_create( size ) );
    if( !goerzHandle.get() )
    {
        throw std::runtime_error( "Could not create Goerzel transform handle" );
    }
    if( !GoerzelTF_precalc( goerzHandle.get(), lowerBound_, upperBound_ ) )
    {
        throw std::runtime_error( "Failed to evaluate precalculation" );
    }

    real_array.resize(size);
    fourier_image.resize( upperBound_ - lowerBound_ );
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class created");
}

Forward::Forward(size_t _size) : Forward( _size, 0, _size/2+1 )
{};

Forward::~Forward()
{
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class destroyed");
}

void Forward::Execute() noexcept
{
    size_t freqIndex = lowerBound_;
    for( auto& harmonica : fourier_image )
    {
        if( !GoerzelTF_set_freq_idx( goerzHandle.get(), freqIndex ) )
        {
            TRACE_EVENT( EVENTS::ERROR, "GoerzelTF_set_freq_idx() failed" );
            return;
        }
        for( auto sample : real_array )
        {
            GoerzelTF_update( goerzHandle.get(), sample );
        }
        harmonica = GoerzelTF_result( goerzHandle.get() );
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
    memcpy(&_fourier[0], &fourier_image[0], sizeof(std::complex<double>)*(upperBound_ - lowerBound_));
}

#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <memory>

#include <FFT/grz_forward_class.hpp>
#include <logger/logger.hpp>
#include <loop_unrolling.h>

using namespace transform::cpu::forward;
using namespace logger;

Goerzel::Goerzel(size_t size, size_t lowerBound, size_t upperBound) :
    size_(size),
    lowerBound_(lowerBound),
    upperBound_(upperBound)
{
    if(4 > size)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }

    if( lowerBound > size_/2u+1u || upperBound_ > size_/2u+1u )
    {
        throw std::logic_error( "Bounds must be less then size+1" );
    }

    if( lowerBound_ >= upperBound_ )
    {
        throw std::logic_error( "Lower bound is greater then upper" );
    }
    goerzHandle_.reset( GoerzelTF_create( size_ ) );
    if( !goerzHandle_.get() )
    {
        throw std::runtime_error( "Could not create Goerzel transform handle" );
    }
    if( !GoerzelTF_precalc( goerzHandle_.get(), lowerBound_, upperBound_ ) )
    {
        throw std::runtime_error( "Failed to evaluate precalculation" );
    }

    fourier_image_.resize( upperBound_ - lowerBound_ );
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class created");
}

Goerzel::Goerzel(size_t size) : Goerzel( size, 0, size/2+1 )
{};

Goerzel::~Goerzel()
{
    TRACE_EVENT(EVENTS::CREATE, "fft_forward class destroyed");
}

void Goerzel::Execute()
{
    Forward::Execute();
}

void Goerzel::Execute( const std::vector< double >& real )
{
    size_t freqIndex = lowerBound_;
    for( auto& harmonica : fourier_image_ )
    {
        GoerzelTF_set_freq_idx( goerzHandle_.get(), freqIndex );

#ifdef ENABLE_LOOP_UNROLLING
        const double* sample = real.data();
        UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, real.size(),
            GoerzelTF_update( goerzHandle_.get(), *sample );
            ++sample;
        )
#else
        for( auto& sample : real )
        {
            GoerzelTF_update( goerzHandle_.get(), sample );
        }
#endif // ENABLE_LOOP_UNROLLING

        harmonica = GoerzelTF_result( goerzHandle_.get() );
        ++freqIndex;
    }
}

void Goerzel::NormalizeFur()
{
    #ifdef ENABLE_LOOP_UNROLLING
    auto harmonica = fourier_image_.data();
    UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, size_/2+1,
        *harmonica /= size_;
        ++harmonica;
    )
#else
    for ( auto& harmonica : fourier_image_ )
    {
        harmonica /= size_;
    }
#endif // ENABLE_LOOP_UNROLLING
}

void Goerzel::Conjugate()
{
#ifdef ENABLE_LOOP_UNROLLING
    auto harmonica = fourier_image_.data();
    UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, fourier_image_.size(),
        *harmonica = std::conj(*harmonica);
        ++harmonica;
    )
#else
    for ( auto& harmonica : fourier_image_ )
    {
        harmonica = std::conj(harmonica);
    }
#endif // ENABLE_LOOP_UNROLLING
}

void Goerzel::SetReal(const std::vector<double>& real)
{
    Forward::SetReal(real);
}

void Goerzel::GetFourierImage(std::vector<std::complex<double>>& fourier) noexcept
{
    memcpy(&fourier[0], &fourier_image_[0], sizeof(std::complex<double>)*(upperBound_ - lowerBound_));
}

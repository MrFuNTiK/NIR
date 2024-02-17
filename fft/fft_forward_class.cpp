#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <FFT/fft_forward_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>
#include <loop_unrolling.h>

using namespace transform::cpu::forward;
using namespace logger;

FFT::FFT(size_t size) :
    size_(size)
{
    if(4 > size_)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }
    if(size_ & (size_-1))
    {
        throw std::logic_error("Window size must be a power of 2");
    }
    real_array_.resize(size_);
    fourier_image_.resize(size_/2+1);
    forward_plan_ = fftw_plan_dft_r2c_1d(size, &real_array_[0], reinterpret_cast<fftw_complex*>(&fourier_image_[0]), FFTW_ESTIMATE);
    TRACE_EVENT(EVENTS::CREATE, "created");
}

FFT::~FFT()
{
    fftw_destroy_plan(forward_plan_);
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void FFT::Execute()
{
    fftw_execute(forward_plan_);
    NormalizeFur();
}

void FFT::Execute(const std::vector<double>& real)
{
    SetReal(real);
    Execute();
}

void FFT::NormalizeFur()
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

void FFT::Conjugate()
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

void FFT::SetReal(const std::vector<double>& real)
{
    memcpy(&real_array_[0], &real[0], sizeof(double)*size_);
}

void FFT::GetFourierImage(std::vector<std::complex<double>>& fourier) noexcept
{
    memcpy(&fourier[0], &fourier_image_[0], sizeof(std::complex<double>)*(size_/2+1));
}

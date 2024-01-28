#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <FFT/fft_forward_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

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
    for (size_t i = 0; i < size_/2+1; ++i)
    {
        fourier_image_[i] /= size_;
    }
}

void FFT::Conjugate()
{
    for (size_t i = 0; i < size_/2+1; ++i)
    {
        fourier_image_[i] = std::conj(fourier_image_[i]);
    }
}

void FFT::SetReal(const std::vector<double>& real)
{
    memcpy(&real_array_[0], &real[0], sizeof(double)*size_);
}

void FFT::GetFourierImage(std::vector<std::complex<double>>& fourier) noexcept
{
    memcpy(&fourier[0], &fourier_image_[0], sizeof(std::complex<double>)*(size_/2+1));
}

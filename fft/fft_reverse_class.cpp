#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <stdexcept>

#include <FFT/fft_reverse_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

using namespace transform::cpu::reverse;
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
    reverse_plan_ = fftw_plan_dft_c2r_1d(size_, reinterpret_cast<fftw_complex*>(&fourier_image_[0]), &real_array_[0], FFTW_ESTIMATE);
    TRACE_EVENT(EVENTS::CREATE, "success");
}

FFT::~FFT()
{
    fftw_destroy_plan(reverse_plan_);
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void FFT::Execute()
{
    fftw_execute(reverse_plan_);
}

void FFT::SetFourierImage(const std::vector<std::complex<double>>& fourier)
{
    memcpy(&fourier_image_[0], &fourier[0], sizeof(std::complex<double>)*(size_/2+1));
}

void FFT::GetReal(std::vector<double>& real)
{
    memcpy(&real[0], &real_array_[0], sizeof(double)*size_);
}

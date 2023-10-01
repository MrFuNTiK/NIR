#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <stdexcept>

#include <FFT/fft_reverse_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

using namespace transform::cpu::reverse;
using namespace logger;

FFT::FFT(size_t _size) :
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
    reverse_plan = fftw_plan_dft_c2r_1d(size, reinterpret_cast<fftw_complex*>(&fourier_image[0]), &real_array[0], FFTW_ESTIMATE);
    TRACE_EVENT(EVENTS::CREATE, "success");
}

FFT::~FFT()
{
    fftw_destroy_plan(reverse_plan);
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void FFT::Execute() noexcept
{
    fftw_execute(reverse_plan);
}

void FFT::SetFourierImage(const std::vector<std::complex<double>>& _fourier) noexcept
{
    memcpy(&fourier_image[0], &_fourier[0], sizeof(std::complex<double>)*(size/2+1));
}

void FFT::GetReal(std::vector<double>& _real) noexcept
{
    memcpy(&_real[0], &real_array[0], sizeof(double)*size);
}

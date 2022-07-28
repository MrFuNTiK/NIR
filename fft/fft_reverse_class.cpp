#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <stdexcept>

#include <FFT/fft_reverse_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

fft_reverse::fft_reverse(uint16_t _size) :
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
    TRACE_EVENT(EVENT, "fft_reverse class created");
}

fft_reverse::~fft_reverse()
{
    fftw_destroy_plan(reverse_plan);
    TRACE_EVENT(EVENT, "fft_reverse class destroyed");
}

void fft_reverse::execute() noexcept
{
    fftw_execute(reverse_plan);
}

void fft_reverse::set_fourier_image(const std::vector<std::complex<double>>& _fourier) noexcept
{
    memcpy(&fourier_image[0], &_fourier[0], sizeof(std::complex<double>)*(size/2+1));
}

void fft_reverse::get_real(std::vector<double>& _real) noexcept
{
    memcpy(&_real[0], &real_array[0], sizeof(double)*size);
}

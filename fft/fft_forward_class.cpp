#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <FFT/fft_forward_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

using namespace transform::cpu::fft;
using namespace logger;

Forward::Forward(uint16_t _size) :
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
    forward_plan = fftw_plan_dft_r2c_1d(size, &real_array[0], reinterpret_cast<fftw_complex*>(&fourier_image[0]), FFTW_ESTIMATE);
    TRACE_EVENT(EVENTS::CREATE, "created");
}

Forward::~Forward()
{
    fftw_destroy_plan(forward_plan);
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void Forward::Execute() noexcept
{
    fftw_execute(forward_plan);
    NormalizeFur();
}

void Forward::NormalizeFur()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i] /= size;
    }
}

void Forward::Conjugate() noexcept
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i] = std::conj(fourier_image[i]);
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

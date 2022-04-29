#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "fft_forward_class.hpp"
#include "core.hpp"

fft_forward::fft_forward(uint16_t _size) :
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
    real_array = new double[size];
    fourier_image = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    forward_plan = fftw_plan_dft_r2c_1d(size, real_array, fourier_image, FFTW_ESTIMATE);
}

fft_forward::~fft_forward()
{
    delete[] real_array;
    fftw_free(fourier_image);
    fftw_destroy_plan(forward_plan);
}

void fft_forward::execute() noexcept
{
    fftw_execute(forward_plan);
    normalize_fur();
}

void fft_forward::normalize_fur()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i][REAL] /= size;
        fourier_image[i][IMAG] /= size;
    }
}

void fft_forward::conjugate() noexcept
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i][IMAG] *= -1;
    }
}

void fft_forward::set_real(double* _real) noexcept
{
    memcpy(real_array, _real, sizeof(double)*size);
}

void fft_forward::get_fourier_image(fftw_complex* _fourier) noexcept
{
    memcpy(_fourier, fourier_image, sizeof(fftw_complex)*(size/2+1));
}

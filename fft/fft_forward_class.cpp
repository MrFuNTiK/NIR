#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <iostream>

#include "fft_forward_class.hpp"
#include "core.hpp"

fft_forward::fft_forward(uint16_t _size) :
    size(_size)
{
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

void fft_forward::execute()
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

void fft_forward::conjugate()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i][IMAG] *= -1;
    }
}

void fft_forward::set_real(double* _real)
{
    memcpy(real_array, _real, sizeof(double)*size);

    /*
    std::cout << "\nGOT ARRAY:\n";
    for (uint16_t i = 0; i < size; ++i)
    {
        std::cout << real_array[i] << std::endl;
    }
     //*/
}

void fft_forward::get_fourier_image(fftw_complex* _fourier)
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        _fourier[i][REAL] = fourier_image[i][REAL];
        _fourier[i][IMAG] = fourier_image[i][IMAG];
    }
}

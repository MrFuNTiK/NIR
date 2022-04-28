#include <cstdint>
#include <fftw3.h>
#include <cstring>
#include <stdexcept>

#include "fft_reverse_class.hpp"
#include "core.hpp"

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

    real_array = new double[size];
    fourier_image = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    reverse_plan = fftw_plan_dft_c2r_1d(size, fourier_image, real_array, FFTW_ESTIMATE);
}

fft_reverse::~fft_reverse()
{
    delete[] real_array;
    fftw_free(fourier_image);
    fftw_destroy_plan(reverse_plan);
}

void fft_reverse::execute()
{
    fftw_execute(reverse_plan);
}

void fft_reverse::set_fourier_image(fftw_complex* _fourier)
{
    /*
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fourier_image[i][REAL] = _fourier[i][REAL];
        fourier_image[i][IMAG] = _fourier[i][IMAG];
    }
    */
    memcpy(fourier_image, _fourier, sizeof(fftw_complex)*(size/2+1));
}

void fft_reverse::get_real(double* _real)
{
    memcpy(_real, real_array, sizeof(double)*size);
}

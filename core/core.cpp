#include <cstdint>
#include <cmath>

#include "core.hpp"

void get_ampl_spectrum(uint16_t size, fftw_complex* fourier_image, double* spectrum)
{
    for (uint16_t i = 0; i < size; ++i)
    {
        spectrum[i] = sqrt(fourier_image[i][REAL]*fourier_image[i][REAL] + fourier_image[i][IMAG]*fourier_image[i][IMAG]);
    }
}

void get_phase_spectrum(uint16_t size, fftw_complex* fourier_image, double* spectrum)
{
    for (uint16_t i = 0; i < size; ++i)
    {
        if (fourier_image[i][REAL] > 0)
        {
            spectrum[i] = atan(fourier_image[i][IMAG]/fourier_image[i][REAL]);
        }
        else if (fourier_image[i][REAL] < 0)
        {
            if (fourier_image[i][IMAG] >= 0)
            {
                spectrum[i] = atan(fourier_image[i][IMAG]/fourier_image[i][REAL]) + M_PI;
            }
            else
            {
                spectrum[i] = atan(fourier_image[i][IMAG]/fourier_image[i][REAL]) - M_PI;
            }
        }
        else
        {
            if (fourier_image[i][IMAG] > 0)
            {
                spectrum[i] = M_PI_2;
            }
            else
            {
                spectrum[i] = M_PI_2;
            }
        }
    }
}

void unwrap_phase_spectrum(uint16_t size, double* phase_spectrum)
{
    for (uint16_t i = 0; i < size - 1; ++i)
    {
        while (abs(phase_spectrum[i] - phase_spectrum[i+1]) > M_PI)
        {
            if (phase_spectrum[i+1] > phase_spectrum[i])
            {
                phase_spectrum[i+1] -= 2*M_PI;
            }
            else
            {
                phase_spectrum[i+1] += 2*M_PI;
            }
        }
    }
}
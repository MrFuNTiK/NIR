#include <cstdint>
#include <cmath>

#include "core.hpp"

void get_ampl_spectrum(const uint16_t size, const std::vector<std::complex<double>>& fourier_image, double* spectrum) noexcept
{
    for (uint16_t i = 0; i < size; ++i)
    {
        spectrum[i] = std::abs(fourier_image[i]);
    }
}

void get_phase_spectrum(const uint16_t size, const std::vector<std::complex<double>>& fourier_image, double* spectrum) noexcept
{
    for (uint16_t i = 0; i < size; ++i)
    {
        spectrum[i] = std::arg(fourier_image[i]);
    }
}

void unwrap_phase_spectrum(const uint16_t size, double* phase_spectrum) noexcept
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

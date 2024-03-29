#include <cstdint>
#include <cmath>

#include "core.hpp"
#include "loop_unrolling.h"

void get_ampl_spectrum(const std::vector<std::complex<double>>& fourier_image,
                       std::vector<double>& spectrum) noexcept
{
#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, fourier_image.size(),
        spectrum[i] = std::abs(fourier_image[i]);
    )
#else
    for (size_t i = 0; i < fourier_image.size(); ++i)
    {
        spectrum[i] = std::abs(fourier_image[i]);
    }
#endif // ENABLE_LOOP_UNROLLING
}

void get_phase_spectrum(const std::vector<std::complex<double>>& fourier_image,
                        std::vector<double>& spectrum) noexcept
{
#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, fourier_image.size(),
        spectrum[i] = std::arg(fourier_image[i]);
    )
#else
    for (size_t i = 0; i < fourier_image.size(); ++i)
    {
        spectrum[i] = std::arg(fourier_image[i]);
    }
#endif // ENABLE_LOOP_UNROLLING
}

void unwrap_phase_spectrum(std::vector<double>& phase_spectrum) noexcept
{
    for (size_t i = 0; i < phase_spectrum.size() - 1; ++i)
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

#ifndef CORE_H
# define CORE_H

# include <fftw3.h>
# include <vector>
# include <complex>

# define REAL 0
# define IMAG 1

template <typename T>

uint8_t swap(T* first, T*second)
{
    if (!first || !second)
        return 1;
    T tmp = *first;
    *first = *second;
    *second = tmp;
    return 0;
}

/// @brief Function calculates amplitude spectrum of given fourier image of signal and writes it to given pointer.
/// Memory for spectrum should be allocated before call of this function.
///
/// @param size             - amount of complex elements in given fourier image
/// @param fourier_image    - given fourier image of signal
/// @param spectrum         - pointer where GPS spectrum will be written
void get_ampl_spectrum(const uint16_t size, const std::vector<std::complex<double>>& fourier_image, double* spectrum);

/// @brief Function calculates phase spectrum of given fourier image of signal and writes it to given pointer.
/// Memory for spectrum should be allocated before call of this function.
///
/// @param size             - amount of complex elements in given fourier image
/// @param fourier_image    - given fourier image of signal
/// @param spectrum         - pointer where GPS spectrum will be written
void get_phase_spectrum(const uint16_t size, const std::vector<std::complex<double>>& fourier_image, double* spectrum);

/// @brief Function unwraps phase spectrum
///
/// @param size             - amount of samples in given spectrum
/// @param phase_spectrum   - pointer to phase spectrum to unwrap
void unwrap_phase_spectrum(const uint16_t size, double* phase_spectrum);

#endif // CORE_H

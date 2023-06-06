#ifndef CORE_H
# define CORE_H

# include <vector>
# include <complex>

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
/// @param fourier_image    given fourier image of signal
/// @param spectrum         pointer where GPS spectrum will be written
void get_ampl_spectrum(const std::vector<std::complex<double>>& fourier_image,
                       std::vector<double>& spectrum) noexcept;

/// @brief Function calculates phase spectrum of given fourier image of signal and writes it to given pointer.
/// Memory for spectrum should be allocated before call of this function.
///
/// @param fourier_image    given fourier image of signal
/// @param spectrum         pointer where GPS spectrum will be written
void get_phase_spectrum(const std::vector<std::complex<double>>& fourier_image,
                        std::vector<double>& spectrum) noexcept;

/// @brief Function unwraps phase spectrum
///
/// @param phase_spectrum   pointer to phase spectrum to unwrap
void unwrap_phase_spectrum(std::vector<double>& phase_spectrum) noexcept;

#endif // CORE_H

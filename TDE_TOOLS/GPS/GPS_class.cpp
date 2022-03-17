#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>

#include "GPS_class.hpp"
#include "fft_forward_class.hpp"
#include "fft_reverse_class.hpp"
#include "core.hpp"

GPS::GPS(uint16_t _size, uint16_t _rate) :
    TDE(_size, _rate),
    forward(_size)
{
    cross_phase_spectrum = new double[size/2+1];
}

GPS::~GPS()
{
    delete[] cross_phase_spectrum;
}

void GPS::update(double* first_, double* second_)
{
    forward.set_real(first_);
    forward.execute();
    forward.get_fourier_image(fur_1);

    forward.set_real(second_);
    forward.execute();
    forward.conjugate();
    forward.get_fourier_image(fur_2);

    get_mul();
    add_mul_to_sum();
}

void GPS::conclude()
{
    double numerator_sum = 0, divider_sum = 0;

    normalize_sum();
    get_phase_spectrum(size/2+1, fur_1_2_sum, cross_phase_spectrum);
    unwrap_phase_spectrum(size/2+1, cross_phase_spectrum);

    //std::cout << "HARMONICAS\tCROSS_PHASE\n";
    for (uint16_t i = 1; i < size/2+1; ++i)
    {
        double harmonica = 2 * M_PI *i * sample_rate / size;
        if (harmonica / (2*M_PI) > BOTTOM_FREQ_BOUND)
        {
            if (harmonica / (2*M_PI) > UPPER_FREQ_BOUND)
                break;
            //std::cout << harmonica / (2*M_PI)<< "\t" << cross_phase_spectrum[i] << std::endl;
            numerator_sum += harmonica * cross_phase_spectrum[i];
            divider_sum += harmonica * harmonica;
        }
    }
    tde = -1 * numerator_sum / divider_sum;
    clear_sum();
}

#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>

#include <TDE/GPS_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <FFT/fft_reverse_class.hpp>
#include <core.hpp>

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

GPS::GPS(uint16_t _size, uint16_t _rate, weighting_func _w_func) :
    TDE_calc(_size, _rate, _w_func),
    forward(_size)
{
    cross_phase_spectrum = new double[size/2+1];
}

GPS::~GPS()
{
    delete[] cross_phase_spectrum;
}

void GPS::update(double* first_, double* second_) noexcept
{
    forward.set_real(first_);
    forward.execute();
    forward.get_fourier_image(reinterpret_cast<fftw_complex*>(&fur_1[0]));

    forward.set_real(second_);
    forward.execute();
    forward.get_fourier_image(reinterpret_cast<fftw_complex*>(&fur_2[0]));

    make_mul_with_conj();
    add_mul_to_sum();

    get_ampl_spectrum(size/2+1, fur_1, &ampl1[0]);
    get_ampl_spectrum(size/2+1, fur_2, &ampl2[0]);

    for( uint32_t i = 0; i < size/2+1; ++i )
    {
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    }
    ++update_count;
}

void GPS::conclude() noexcept
{
    double numerator_sum = 0, divider_sum = 0;

    normalize_sum();
    double w_func_numerator[size/2+1] = { 0 }, w_func_denominator[size/2+1] = { 0 };
    switch (w_func)
    {
    case COHERENCE:
    {
        get_ampl_spectrum(size/2+1, fur_1_2_sum, w_func_numerator);
        for( uint16_t i = 0; i < size/2+1; ++i )
        {
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
        }
    }
    case NONE:
    {

    }
    }
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
            switch(w_func)
            {
            case COHERENCE:
            {
                cross_phase_spectrum[i] *= w_func_numerator[i] / w_func_denominator[i];
                numerator_sum += harmonica * cross_phase_spectrum[i];
                divider_sum += harmonica * harmonica;
            }
            case NONE:
            {
                numerator_sum += harmonica * cross_phase_spectrum[i];
                divider_sum += harmonica * harmonica;
            }
            }
        }
    }
    tde = -1 * numerator_sum / divider_sum;
    clear_inner();
    update_count = 0;
}

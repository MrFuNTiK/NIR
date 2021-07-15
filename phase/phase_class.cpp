#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>

#include "phase_class.hpp"
#include "fft_forward_class.hpp"
#include "fft_reverse_class.hpp"
#include "core.hpp"

phase::phase(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate) :
    avrg_win_num(_avrg_win_num),
    size(_size),
    sample_rate(_rate),
    tde(0)
{
    first_array = new double*[avrg_win_num];
    second_array = new double*[avrg_win_num];

    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        first_array[i] = new double[size];
        second_array[i] = new double[size];
    }

    fur_1           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_2           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2         = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2_sum     = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    clear_sum();
    cross_phase_spectrum = new double[size/2+1];
}

phase::~phase()
{
    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        delete[] first_array[i];
        delete[] second_array[i];
    }
    delete[] first_array;
    delete[] second_array;

    fftw_free(fur_1);
    fftw_free(fur_2);
    fftw_free(fur_1_2);
    fftw_free(fur_1_2_sum);

    delete[] cross_phase_spectrum;
}

void phase::set_arrays(double **_first, double **_second)
{
    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        memcpy(first_array[i], _first[i], sizeof(double)*size);
        memcpy(second_array[i], _second[i], sizeof(double)*size);
    }
}

void phase::get_mul()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2[i][REAL] = fur_1[i][REAL]*fur_2[i][REAL] - fur_1[i][IMAG]*fur_2[i][IMAG];
        fur_1_2[i][IMAG] = fur_1[i][REAL]*fur_2[i][IMAG] + fur_1[i][IMAG]*fur_2[i][REAL];
    }
}

void phase::clear_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] = 0;
        fur_1_2_sum[i][IMAG] = 0;
    }
}

void phase::add_mul_to_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] += fur_1_2[i][REAL];
        fur_1_2_sum[i][IMAG] += fur_1_2[i][IMAG];
    }
}

void phase::normalize_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] /= size;
        fur_1_2_sum[i][IMAG] /= size;
    }
}

void phase::execute()
{
    fft_forward forward_1(size);
    fft_forward forward_2(size);
    fft_reverse reverse(size);

    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        forward_1.set_real(first_array[i]);
        forward_2.set_real(second_array[i]);

        forward_1.execute();
        forward_2.execute();
        forward_1.conjugate();

        forward_1.get_fourier_image(fur_1);
        forward_2.get_fourier_image(fur_2);

        get_mul();
        add_mul_to_sum();
    }
    normalize_sum();

    get_phase_spectrum(size/2+1, fur_1_2_sum, cross_phase_spectrum);
    unwrap_phase_spectrum(size/2+1, cross_phase_spectrum);

    clear_sum();
}

void phase::calculate_tde()
{
    double numerator_sum = 0, divider_sum = 0;
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
}

double phase::get_tde()
{
    return tde;
}

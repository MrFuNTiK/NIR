#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "correllation_class.hpp"
#include "fft_forward_class.hpp"
#include "fft_reverse_class.hpp"
#include "core.hpp"


correlation::correlation(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate) :
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

    corr_func = new double[size];

    fur_1           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_2           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2         = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2_sum     = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    clear_sum();
}

correlation::~correlation()
{
    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        delete[] first_array[i];
        delete[] second_array[i];
    }
    delete[] first_array;
    delete[] second_array;

    delete[] corr_func;

    fftw_free(fur_1);
    fftw_free(fur_2);
    fftw_free(fur_1_2);
    fftw_free(fur_1_2_sum);
}

void correlation::set_arrays(double** _first, double** _second)
{
    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        memcpy(first_array[i], _first[i], sizeof(double)*size);
        memcpy(second_array[i], _second[i], sizeof(double)*size);
    }
}

void correlation::get_corr_func(double* _corr)
{
    memcpy(_corr, corr_func, sizeof(double)*size);
}

void correlation::get_mul()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2[i][REAL] = fur_1[i][REAL]*fur_2[i][REAL] - fur_1[i][IMAG]*fur_2[i][IMAG];
        fur_1_2[i][IMAG] = fur_1[i][REAL]*fur_2[i][IMAG] + fur_1[i][IMAG]*fur_2[i][REAL];
    }
}

void correlation::clear_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] = 0;
        fur_1_2_sum[i][IMAG] = 0;
    }
}

void correlation::add_mul_to_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] += fur_1_2[i][REAL];
        fur_1_2_sum[i][IMAG] += fur_1_2[i][IMAG];
    }
}

void correlation::normalize_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] /= size;
        fur_1_2_sum[i][IMAG] /= size;
    }
}

void correlation::shift_corr_func()
{
    for (uint16_t i = 0; i < size/2; ++i)
    {
        for (uint16_t j = 0; j < size - 1; ++j)
        {
            swap<double>(&corr_func[j], &corr_func[j+1]);
        }
    }
}

void correlation::execute()
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

    reverse.set_fourier_image(fur_1_2_sum);
    reverse.execute();
    reverse.get_real(corr_func);
    shift_corr_func();
    clear_sum();
}

void correlation::calculate_tde()
{
    double corr_max = corr_func[0];
    int16_t num_max = 0;
    for (uint16_t i = 1; i < size; ++i)
    {
        if (corr_func[i] > corr_max)
        {
            corr_max = corr_func[i];
            num_max = static_cast<int>(i);
        }
    }
    num_max -= 512;
    tde = num_max/static_cast<double>(sample_rate);
}

double correlation::get_tde()
{
    return tde;
}
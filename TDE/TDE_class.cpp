#include <cstring>
#include "TDE_class.hpp"
#include "core.hpp"

TDE::TDE(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate) :
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
}

TDE::~TDE()
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
}

void TDE::set_arrays(double** _first, double** _second)
{
    for (uint16_t i = 0; i < avrg_win_num; ++i)
    {
        memcpy(first_array[i], _first[i], sizeof(double)*size);
        memcpy(second_array[i], _second[i], sizeof(double)*size);
    }
}

void TDE::get_mul()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2[i][REAL] = fur_1[i][REAL]*fur_2[i][REAL] - fur_1[i][IMAG]*fur_2[i][IMAG];
        fur_1_2[i][IMAG] = fur_1[i][REAL]*fur_2[i][IMAG] + fur_1[i][IMAG]*fur_2[i][REAL];
    }
}

void TDE::clear_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] = 0;
        fur_1_2_sum[i][IMAG] = 0;
    }
}

void TDE::add_mul_to_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] += fur_1_2[i][REAL];
        fur_1_2_sum[i][IMAG] += fur_1_2[i][IMAG];
    }
}

void TDE::normalize_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] /= size;
        fur_1_2_sum[i][IMAG] /= size;
    }
}

double TDE::get_tde()
{
    return tde;
}
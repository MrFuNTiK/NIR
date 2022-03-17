#include <cstring>
#include "TDE_class.hpp"
#include "core.hpp"

TDE::TDE(uint16_t _size, uint16_t _rate) :
    size(_size),
    sample_rate(_rate),
    tde(0)
{
    fur_1           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_2           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2         = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2_sum     = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    clear_sum();
}

TDE::~TDE()
{
    fftw_free(fur_1);
    fftw_free(fur_2);
    fftw_free(fur_1_2);
    fftw_free(fur_1_2_sum);

    fftw_cleanup();
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

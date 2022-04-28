#include <cstring>
#include <stdexcept>

#include <TDE_class.hpp>
#include <core.hpp>

TDE_calc::TDE_calc(uint16_t _size, uint16_t _rate, weighting_func _w_func) :
    size(_size),
    sample_rate(_rate),
    w_func(_w_func),
    update_count(0),
    tde(0)
{
    if(4 > size)
    {
        throw std::logic_error("Window size must be greater or equal 4");
    }
    if(size & (size-1))
    {
        throw std::logic_error("Window size must be a power of 2");
    }

    if(0 == sample_rate)
    {
        throw std::logic_error("Sampling rate must be greater than 0");
    }

    fur_1           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_2           = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2         = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));
    fur_1_2_sum     = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(size/2+1));

    ampl1           = new double[size/2+1];
    ampl2           = new double[size/2+1];
    ampl1_sum       = new double[size/2+1];
    ampl2_sum       = new double[size/2+1];
    clear_inner();
}

TDE_calc::~TDE_calc()
{
    fftw_free(fur_1);
    fftw_free(fur_2);
    fftw_free(fur_1_2);
    fftw_free(fur_1_2_sum);

    delete[] ampl1;
    delete[] ampl2;
    delete[] ampl1_sum;
    delete[] ampl2_sum;

    fftw_cleanup();
}

void TDE_calc::make_mul()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2[i][REAL] = fur_1[i][REAL]*fur_2[i][REAL] - fur_1[i][IMAG]*fur_2[i][IMAG];
        fur_1_2[i][IMAG] = fur_1[i][REAL]*fur_2[i][IMAG] + fur_1[i][IMAG]*fur_2[i][REAL];
    }
}

void TDE_calc::make_mul_with_conj()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2[i][REAL] = fur_1[i][REAL]*fur_2[i][REAL] + fur_1[i][IMAG]*fur_2[i][IMAG];
        fur_1_2[i][IMAG] = -fur_1[i][REAL]*fur_2[i][IMAG] + fur_1[i][IMAG]*fur_2[i][REAL];
    }
}

void TDE_calc::clear_inner()
{
    std::memset(fur_1_2_sum, 0, sizeof( fftw_complex ) * (size / 2 + 1));
    std::memset(ampl1_sum, 0, sizeof(double) * (size / 2 + 1));
    std::memset(ampl2_sum, 0, sizeof(double) * (size / 2 + 1));
}

void TDE_calc::add_mul_to_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] += fur_1_2[i][REAL];
        fur_1_2_sum[i][IMAG] += fur_1_2[i][IMAG];
    }
}

void TDE_calc::normalize_sum()
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] /= update_count;
        fur_1_2_sum[i][IMAG] /= update_count;
        ampl1_sum[i] /= update_count;
        ampl2_sum[i] /= update_count;
    }
}

double TDE_calc::get_tde()
{
    return tde;
}

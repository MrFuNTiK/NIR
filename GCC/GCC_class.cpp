#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "GCC_class.hpp"
#include "fft_forward_class.hpp"
#include "fft_reverse_class.hpp"
#include "core.hpp"


GCC::GCC(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate) :
    TDE(_avrg_win_num, _size, _rate)
{
    corr_func = new double[size];
    PHAT_func = new double[size/2+1];
}

GCC::~GCC()
{
    delete[] corr_func;
    delete[] PHAT_func;
}

void GCC::get_corr_func(double* _corr)
{
    memcpy(_corr, corr_func, sizeof(double)*size);
}

void GCC::shift_corr_func()
{
    for (uint16_t i = 0; i < size/2; ++i)
    {
        for (uint16_t j = 0; j < size - 1; ++j)
        {
            swap<double>(&corr_func[j], &corr_func[j+1]);
        }
    }
}

void GCC::apply_weight_func(double* weight_func)
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i][REAL] /= weight_func[i];
        fur_1_2_sum[i][IMAG] /= weight_func[i];
    }
}

void GCC::execute()
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
    get_ampl_spectrum(size/2+1, fur_1_2_sum, PHAT_func);
    apply_weight_func(PHAT_func);

    reverse.set_fourier_image(fur_1_2_sum);
    reverse.execute();
    reverse.get_real(corr_func);
    shift_corr_func();
    clear_sum();
}

void GCC::calculate_tde()
{
    double corr_max = corr_func[0];
    int16_t num_max = 0;
    for (uint16_t i = 1; i < size; ++i)
    {
        std::cout << corr_func[i] << std::endl;
        if (corr_func[i] > corr_max)
        {
            corr_max = corr_func[i];
            num_max = static_cast<int>(i);
        }
    }
    num_max -= 512;
    tde = num_max/static_cast<double>(sample_rate);
}

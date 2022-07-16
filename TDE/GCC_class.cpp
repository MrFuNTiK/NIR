#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>

#include <TDE/GCC_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <FFT/fft_reverse_class.hpp>
#include <core.hpp>


GCC::GCC(uint16_t _size, uint16_t _rate, weighting_func _w_func) :
    TDE_calc(_size, _rate, _w_func),
    forward(_size),
    reverse(_size)
{
    corr_func.resize(size);
    PHAT_func.resize(size/2+1);
}

GCC::~GCC()
{}

void GCC::get_corr_func(std::vector<double>& _corr)
{
    memcpy(&_corr[0], &corr_func[0], sizeof(double)*size);
}

void GCC::shift_corr_func()
{
    for (uint16_t i = 0; i < size/2; ++i)
    {
        swap<double>(&corr_func[i], &corr_func[i + size/2]);
    }
}

void GCC::apply_PHAT_func(double* weight_func)
{
    for (uint16_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i] /= weight_func[i];
    }
}

void GCC::update(const std::vector<double>& first_, const std::vector<double>& second_) noexcept
{
    forward.set_real(first_);
    forward.execute();
    forward.get_fourier_image(fur_1);

    forward.set_real(second_);
    forward.execute();
    forward.get_fourier_image(fur_2);

    make_mul_with_conj();
    add_mul_to_sum();

    get_ampl_spectrum(size/2+1, fur_1, &ampl1[0]);
    get_ampl_spectrum(size/2+1, fur_2, &ampl2[0]);

    for( uint32_t i = 0; i < size/2u+1u; ++i )
    {
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    }

    ++update_count;
}

void GCC::conclude() noexcept
{
    normalize_sum();

    std::vector<double> w_func_numerator(size/2+1);
    std::vector<double> w_func_denominator(size/2+1);
    switch (w_func)
    {
    case COHERENCE:
    {
        get_ampl_spectrum(size/2+1, fur_1_2_sum, w_func_numerator.data());
        for( uint16_t i = 0; i < size/2+1; ++i )
        {
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
            fur_1_2_sum[i] *= w_func_numerator[i] / w_func_denominator[i];
        }
        break;
    }
    case NONE:
    {
        break;
    }
    }

    reverse.set_fourier_image(fur_1_2_sum);
    reverse.execute();
    reverse.get_real(corr_func);
    shift_corr_func();
    clear_inner();


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
    num_max -= size/2;
    tde = num_max/static_cast<double>(sample_rate);

    update_count = 0;
}

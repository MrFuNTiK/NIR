#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>

#include <TDE/GCC_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <FFT/fft_reverse_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>
#include <loop_unrolling.h>

using namespace tde::gcc;
using namespace logger;

GCC::GCC(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func) :
    iTDE(_size, _rate, _w_func),
    forward(_size),
    reverse(_size)
{
    fur_1.resize(size/2+1);
    fur_2.resize(size/2+1);
    fur_1_2.resize(size/2+1);
    fur_1_2_sum.resize(size/2+1);

    ampl1.resize(size/2+1);
    ampl2.resize(size/2+1);
    ampl1_sum.resize(size/2+1);
    ampl2_sum.resize(size/2+1);

    corr_func.resize(size);
    PHAT_func.resize(size/2+1);

    clear_inner();
    TRACE_EVENT(EVENTS::CREATE, "success");
}

GCC::~GCC()
{
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void GCC::GetCorrFunc(std::vector<double>& _corr)
{
    memcpy(&_corr[0], &corr_func[0], sizeof(double)*size);
}

void GCC::shift_corr_func()
{
#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_THIRTY_TWO, i, 0, size/2,
        swap<double>(&corr_func[i], &corr_func[i + size/2]);
    )
#else
    for (size_t i = 0; i < size/2; ++i)
    {
        swap<double>(&corr_func[i], &corr_func[i + size/2]);
    }
#endif // ENABLE_LOOP_UNROLLING
}

void GCC::apply_PHAT_func(double* weight_func)
{
#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_THIRTY_TWO, i, 0, size/2+1,
        fur_1_2_sum[i] /= weight_func[i];
    )
#else
    for (size_t i = 0; i < size/2+1; ++i)
    {
        fur_1_2_sum[i] /= weight_func[i];
    }
#endif // ENABLE_LOOP_UNROLLING
}

void GCC::Update(const std::vector<double>& first_, const std::vector<double>& second_)
{
    TRACE_EVENT( EVENTS::TDE_CALC, "called" );
    forward.SetReal(first_);
    forward.Execute();
    forward.GetFourierImage(fur_1);

    forward.SetReal(second_);
    forward.Execute();
    forward.GetFourierImage(fur_2);

    make_mul_with_conj();
    add_mul_to_sum();

    get_ampl_spectrum(fur_1, ampl1);
    get_ampl_spectrum(fur_2, ampl2);

#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_THIRTY_TWO, i, 0, size/2+1,
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    )
#else
    for( uint32_t i = 0; i < size/2u+1u; ++i )
    {
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    }
#endif // ENABLE_LOOP_UNROLLING

    ++update_count;
}

void GCC::Conclude()
{
    TRACE_EVENT( EVENTS::TDE_CALC, "called" );

    normalize_sum();

    std::vector<double> w_func_numerator(size/2+1);
    std::vector<double> w_func_denominator(size/2+1);
    switch (w_func)
    {
    case COHERENCE:
    {
        get_ampl_spectrum(fur_1_2_sum, w_func_numerator);
#ifdef ENABLE_LOOP_UNROLLING
        UNROLL_LOOP( UNROLL_FACTOR_THIRTY_TWO, i, 0, size/2+1,
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
            fur_1_2_sum[i] *= w_func_numerator[i] / w_func_denominator[i];
        )
#else
        for( size_t i = 0; i < size/2+1; ++i )
        {
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
            fur_1_2_sum[i] *= w_func_numerator[i] / w_func_denominator[i];
        }
#endif // ENABLE_LOOP_UNROLLING
        break;
    }
    case NONE:
    {
        break;
    }
    }

    reverse.SetFourierImage(fur_1_2_sum);
    reverse.Execute();
    reverse.GetReal(corr_func);
    shift_corr_func();
    clear_inner();


    double corr_max = corr_func[0];
    int num_max = 0;
#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_THIRTY_TWO, i, 1, size,
        if (corr_func[i] > corr_max)
        {
            corr_max = corr_func[i];
            num_max = static_cast<int>(i);
        }
    )
#else
    for (size_t i = 1; i < size; ++i)
    {
        if (corr_func[i] > corr_max)
        {
            corr_max = corr_func[i];
            num_max = static_cast<int>(i);
        }
    }
#endif // ENABLE_LOOP_UNROLLING
    num_max -= size/2;
    tde = num_max/static_cast<double>(sample_rate);

    update_count = 0;
}

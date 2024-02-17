#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>

#include <TDE/GPS_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <FFT/fft_reverse_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>
#include <loop_unrolling.h>

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

using namespace tde::gps;
using namespace logger;

GPS::GPS(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func) :
    iTDE(_size, _rate, _w_func),
    forward(_size)
{
    fur_1.resize(size/2+1);
    fur_2.resize(size/2+1);
    fur_1_2.resize(size/2+1);
    fur_1_2_sum.resize(size/2+1);

    ampl1.resize(size/2+1);
    ampl2.resize(size/2+1);
    ampl1_sum.resize(size/2+1);
    ampl2_sum.resize(size/2+1);

    cross_phase_spectrum.resize(size/2+1);

    clear_inner();
    TRACE_EVENT(EVENTS::CREATE, "success");
}

GPS::~GPS()
{
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void GPS::Update(const std::vector<double>& first_, const std::vector<double>& second_)
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
    for( size_t i = 0; i < size/2+1; ++i )
    {
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    }
#endif // ENABLE_LOOP_UNROLLING
    ++update_count;
}

void GPS::Conclude()
{
    double numerator_sum = 0, divider_sum = 0;

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
        )
#else
        for( size_t i = 0; i < size/2+1; ++i )
        {
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
        }
#endif // ENABLE_LOOP_UNROLLING
        break;
    }
    case NONE:
    {
        break;
    }
    }
    get_phase_spectrum(fur_1_2_sum, cross_phase_spectrum);
    unwrap_phase_spectrum(cross_phase_spectrum);

    //std::cout << "HARMONICAS\tCROSS_PHASE\n";
    for (size_t i = 1; i < size/2+1; ++i)
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
                break;
            }
            case NONE:
            {
                numerator_sum += harmonica * cross_phase_spectrum[i];
                divider_sum += harmonica * harmonica;
                break;
            }
            }
        }
    }
    tde = -1 * numerator_sum / divider_sum;
    clear_inner();
    update_count = 0;
}

#include <fftw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>

#include <TDE/GPS_GRZ_class.hpp>
#include <FFT/grz_forward_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

using namespace tde::gps;
using namespace logger;

GPS_GRZ::GPS_GRZ(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func) :
    iTDE(_size, _rate, _w_func),
    forward(_size,
            (size/2+1.) / sample_rate * BOTTOM_FREQ_BOUND,
            (size/2+1.) / sample_rate * UPPER_FREQ_BOUND)
{
    lowerBound = (size/2+1.) / sample_rate * BOTTOM_FREQ_BOUND;
    upperBound = (size/2+1.) / sample_rate * UPPER_FREQ_BOUND;

    auto diff = upperBound - lowerBound;
    TRACE_EVENT( EVENTS::DEBUG, std::to_string( diff ).c_str() );

    fur_1.resize(diff);
    fur_2.resize(diff);
    fur_1_2.resize(diff);
    fur_1_2_sum.resize(diff);

    ampl1.resize(diff);
    ampl2.resize(diff);
    ampl1_sum.resize(diff);
    ampl2_sum.resize(diff);

    cross_phase_spectrum.resize(diff);

    clear_inner();
    TRACE_EVENT(EVENTS::CREATE, "success");
}

GPS_GRZ::~GPS_GRZ()
{
    TRACE_EVENT(EVENTS::CREATE, "destroyed");
}

void GPS_GRZ::Update(const std::vector<double>& first_, const std::vector<double>& second_)
{
    TRACE_EVENT( EVENTS::TDE_CALC, "called" );
    //forward.SetReal(first_);
    forward.Execute(first_);
    forward.GetFourierImage(fur_1);

    //forward.SetReal(second_);
    forward.Execute(second_);
    forward.GetFourierImage(fur_2);

    make_mul_with_conj();
    add_mul_to_sum();

    get_ampl_spectrum(fur_1, ampl1);
    get_ampl_spectrum(fur_2, ampl2);

    for( size_t i = 0; i < ampl1_sum.size(); ++i )
    {
        ampl1_sum[i] += ampl1[i] * ampl1[i];
        ampl2_sum[i] += ampl2[i] * ampl2[i];
    }
    ++update_count;
}

void GPS_GRZ::Conclude()
{
    double numerator_sum = 0, divider_sum = 0;

    TRACE_EVENT( EVENTS::TDE_CALC, "called" );

    normalize_sum();
    std::vector<double> w_func_numerator(ampl1_sum.size());
    std::vector<double> w_func_denominator(ampl1_sum.size());
    switch (w_func)
    {
    case COHERENCE:
    {
        get_ampl_spectrum(fur_1_2_sum, w_func_numerator);
        for( size_t i = 0; i < w_func_numerator.size(); ++i )
        {
            w_func_denominator[i] = ampl1_sum[i] * ampl2_sum[i];
            w_func_numerator[i] *= w_func_numerator[i];
        }
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
    for (size_t i = 1; i < upperBound - lowerBound; ++i)
    {
        double harmonica = 2 * M_PI *i * sample_rate / size;
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
    tde = -1 * numerator_sum / divider_sum;
    clear_inner();
    update_count = 0;
}

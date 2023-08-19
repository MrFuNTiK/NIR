#include <cstring>
#include <stdexcept>
#include <sstream>
#include <fftw3.h>

#include <TDE/TDE_class.hpp>
#include <logger/logger.hpp>
#include <core.hpp>

namespace tde
{

iTDE::iTDE(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func) :
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
}

iTDE::~iTDE()
{
    fftw_cleanup();
}

void iTDE::make_mul()
{
    if( fur_1.size() != fur_2.size() || fur_1.size() != fur_1_2.size() )
    {
        TRACE_EVENT( logger::EVENTS::ERROR, "Inconsistent sizes of fourier buffers" );
    }

    for (size_t i = 0; i < fur_1_2.size(); ++i)
    {
        fur_1_2[i] = fur_1[i] * fur_2[i];
    }
}

void iTDE::make_mul_with_conj()
{
    if( fur_1.size() != fur_2.size() || fur_1.size() != fur_1_2.size() )
    {
        TRACE_EVENT( logger::EVENTS::ERROR, "Inconsistent sizes of fourier buffers" );
    }

    for (size_t i = 0; i < fur_1_2.size(); ++i)
    {
        fur_1_2[i] = fur_1[i] * std::conj(fur_2[i]);
    }
}

void iTDE::clear_inner()
{
    std::fill(fur_1_2_sum.begin(), fur_1_2_sum.end(), 0);
    std::fill(ampl1_sum.begin(), ampl1_sum.end(), 0);
    std::fill(ampl2_sum.begin(), ampl2_sum.end(), 0);
}

void iTDE::add_mul_to_sum()
{
    for (size_t i = 0; i < fur_1_2_sum.size(); ++i)
    {
        fur_1_2_sum[i] += fur_1_2[i];
    }
}

void iTDE::normalize_sum()
{
    if( ampl1_sum.size() != ampl2_sum.size() || fur_1_2_sum.size() != ampl1_sum.size() )
    {
        TRACE_EVENT( logger::EVENTS::ERROR, "Inconsistent sizes of amplitude and fourier buffers" );
    }
    for (size_t i = 0; i < fur_1_2_sum.size(); ++i)
    {
        fur_1_2_sum[i] /= update_count;
        ampl1_sum[i] /= update_count;
        ampl2_sum[i] /= update_count;
    }
}

double iTDE::GetTde()
{
    return tde;
}

std::string WEIGHTING_FN_TYPE_to_str( WEIGHTING_FN_TYPE type )
{
    switch( type )
    {
    case WEIGHTING_FN_TYPE::COHERENCE:
    {
        return COHERENCE_WEIGHT_FN_STR;
    }
    case WEIGHTING_FN_TYPE::NONE:
    {
        return NONE_WEIGHTING_FN_STR;
    }
    default:
    {
        throw std::runtime_error( "Undefined weighting function type" );
    }
    }
}

WEIGHTING_FN_TYPE WEIGHTING_FN_TYPE_from_str( std::string& str )
{
    if( str.empty() )
    {
        throw std::runtime_error( "String is empty" );
    }

    if( str == COHERENCE_WEIGHT_FN_STR )
    {
        return WEIGHTING_FN_TYPE::COHERENCE;
    }
    else if( str == NONE_WEIGHTING_FN_STR )
    {
        return WEIGHTING_FN_TYPE::NONE;
    }
    else
    {
        std::stringstream error;
        error << "Undefined weghting function: " << str;
        throw std::runtime_error( error.str() );
    }
}

} // namespace tde

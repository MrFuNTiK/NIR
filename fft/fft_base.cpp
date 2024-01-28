#include <FFT/fft_base.hpp>

using namespace transform;

void Forward::Execute()
{
    throw std::logic_error( "Not supported" );
}

void Forward::Execute( const std::vector<double>& real )
{
    ( void )real;
    throw std::logic_error( "Not supported" );
}

void Forward::Conjugate()
{
    throw std::logic_error("Not supported");
}

void Forward::SetReal( const std::vector<double>& real )
{
    ( void )real;
    throw std::logic_error( "Not supported" );
}

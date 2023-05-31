#include <iostream>
#include <complex>
#include <gtest/gtest.h>
#include "goerzel.h"

TEST( GoerzTest, test )
{
    const size_t SAMPLES = 1 << 5;

    std::array<double, SAMPLES> signal;
    signal.fill( 10. );

    [[maybe_unused]]double sine[ SAMPLES ];
    for( size_t i = 0; i < SAMPLES; ++i )
    {
        sine[ i ] = 10 * sin( 4 * 2 * M_PI * i / SAMPLES);
    }

    GoerzelTF* tf = GoerzelTF_create( SAMPLES );

    for( size_t i = 0; i < SAMPLES / 2 + 1 ; ++i )
    {
        GoerzelTF_set_freq_idx( tf, i );
        ASSERT_NE( tf, nullptr );
        for( auto sample : sine )
        {
            ASSERT_EQ( 1, GoerzelTF_update( tf, sample ) );
        }
        std::complex<double> res = GoerzelTF_result( tf );
        std::cout << std::abs(res) << "\t" << std::arg(res) << std::endl;
    }
    GoerzelTF_destroy( tf );
}

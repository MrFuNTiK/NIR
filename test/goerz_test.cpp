#include <iostream>
#include <complex>
#include <memory>
#include <gtest/gtest.h>
#include "goerzel.h"

using GoerzPtr = std::unique_ptr< GoerzelTF, decltype( &GoerzelTF_destroy ) >;

TEST( GoerzTest, test )
{
    const size_t SAMPLES = 1 << 10;

    double sine[ SAMPLES ];
    for( size_t i = 0; i < SAMPLES; ++i )
    {
        sine[ i ] = 10 * sin( 4 * 2 * M_PI * i / SAMPLES);
    }

    GoerzPtr tf( GoerzelTF_create( SAMPLES ), GoerzelTF_destroy );

    for( size_t i = 0; i < SAMPLES / 2 + 1 ; ++i )
    {
        ASSERT_EQ( 1, GoerzelTF_set_freq_idx( tf.get(), i ) );
        ASSERT_NE( tf, nullptr );
        for( auto sample : sine )
        {
            ASSERT_EQ( 1, GoerzelTF_update( tf.get(), sample ) );
        }
        std::complex<double> res = GoerzelTF_result( tf.get() );
        // Check for NaN
        ASSERT_EQ( res, res );
    }
}

#include <iostream>
#include <complex>
#include <memory>
#include <gtest/gtest.h>
#include <FFT/fft_forward_class.hpp>
#include <FFT/grz_forward_class.hpp>
#include "goerzel.h"

#include <FFT/sfft_class.hpp>

static const size_t SAMPLES = 1 << 5;
static const double AMPLITUDE = 20;

TEST( GoerzTest, UpdateResultExpectSuccess )
{
    const size_t SAMPLES = 1 << 4;

    double sine[ SAMPLES ];
    for( size_t i = 0; i < SAMPLES; ++i )
    {
        sine[ i ] = 10 * sin( 3 * 2 * M_PI * i / SAMPLES);
    }

    transform::cpu::forward::GoerzPtr tf( GoerzelTF_create( SAMPLES ), GoerzelTF_destroy );
    ASSERT_EQ( 1, GoerzelTF_precalc( tf.get(), 0, SAMPLES / 2 + 1 ) );

    for( size_t i = 0; i < SAMPLES / 2 + 1 ; ++i )
    {
        GoerzelTF_set_freq_idx( tf.get(), i );
        for( auto sample : sine )
        {
            GoerzelTF_update( tf.get(), sample );
        }
        std::complex<double> res = GoerzelTF_result( tf.get() );
        ASSERT_EQ( res, res );
    }
}

TEST( GoerzTest, Goerzel_VS_fftw3 )
{
    transform::cpu::forward::FFT fft( SAMPLES );
    transform::cpu::forward::Goerzel grz( SAMPLES, 0, SAMPLES / 2 + 1 );

    std::vector<double> sine( SAMPLES );
    for( size_t i = 0; i < SAMPLES; ++i )
    {
        sine[ i ] = AMPLITUDE * cos( 2 * 2 * M_PI * i / SAMPLES + M_PI/4) +
                    AMPLITUDE * cos( 5 * 2 * M_PI * i / SAMPLES - M_PI/2 ) +
                    AMPLITUDE * cos( 10 * 2 * M_PI * i / SAMPLES + M_PI/4);
    }

    std::vector< std::complex< double > > fftRes( SAMPLES / 2 + 1 );
    std::vector< std::complex< double > > grzRes( SAMPLES / 2 + 1 );

    fft.SetReal( sine );
    fft.Execute();
    fft.Execute();
    fft.GetFourierImage( fftRes );

    grz.SetReal( sine );
    grz.Execute();
    grz.Execute();
    grz.GetFourierImage( grzRes );

    for( size_t i = 0; i < fftRes.size(); ++i )
    {
        if( i == 2 || i == 5 || i == 10 )
        {
            EXPECT_NEAR( fftRes[ i ].real(), grzRes[ i ].real(), 0.000001 );
            EXPECT_NEAR( fftRes[ i ].imag(), grzRes[ i ].imag(), 0.000001 );
            EXPECT_NEAR( abs( fftRes[ i ] ), abs( grzRes[ i ] ), 0.000001 );
            EXPECT_NEAR( arg( fftRes[ i ] ), arg( grzRes[ i ] ), 0.000001 );

            EXPECT_NEAR( abs( fftRes[ i ] ), AMPLITUDE / 2, 0.000001 );
            EXPECT_NEAR( abs( grzRes[ i ] ), AMPLITUDE / 2, 0.000001 );
        }
        else
        {
            EXPECT_NEAR( abs( fftRes[ i ] ), 0., 0.000001 );
            EXPECT_NEAR( abs( grzRes[ i ] ), 0., 0.000001 );
        }
    }
}

TEST( GoerzTest, Sliding_VS_fftw3 )
{
    transform::cpu::forward::FFT fft( SAMPLES );
    transform::cpu::forward::SFFT sfft( SAMPLES );

    std::vector<double> sine( SAMPLES );
    for( size_t i = 0; i < SAMPLES; ++i )
    {
        sine[ i ] = AMPLITUDE * cos( 2 * 2 * M_PI * i / SAMPLES + M_PI/4) +
                    AMPLITUDE * cos( 5 * 2 * M_PI * i / SAMPLES - M_PI/2 ) +
                    AMPLITUDE * cos( 10 * 2 * M_PI * i / SAMPLES + M_PI/4);
    }

    std::vector< std::complex< double > > fftRes( SAMPLES / 2 + 1 );
    std::vector< std::complex< double > > sfftRes( SAMPLES / 2 + 1 );

    fft.SetReal( sine );
    fft.Execute();
    fft.Execute();
    fft.GetFourierImage( fftRes );

    sfft.SetReal( sine );
    sfft.Execute();
    sfft.Execute();
    sfft.GetFourierImage( sfftRes );

    for( size_t i = 0; i < fftRes.size(); ++i )
    {
        if( i == 2 || i == 5 || i == 10 )
        {
            EXPECT_NEAR( fftRes[ i ].real(), sfftRes[ i ].real(), 0.000001 );
            EXPECT_NEAR( fftRes[ i ].imag(), sfftRes[ i ].imag(), 0.000001 );
            EXPECT_NEAR( abs( fftRes[ i ] ), abs( sfftRes[ i ] ), 0.000001 );
            EXPECT_NEAR( arg( fftRes[ i ] ), arg( sfftRes[ i ] ), 0.000001 );

            EXPECT_NEAR( abs( fftRes[ i ] ), AMPLITUDE / 2, 0.000001 );
            EXPECT_NEAR( abs( sfftRes[ i ] ), AMPLITUDE / 2, 0.000001 );
        }
        else
        {
            EXPECT_NEAR( abs( fftRes[ i ] ), 0., 0.000001 );
            EXPECT_NEAR( abs( sfftRes[ i ] ), 0., 0.000001 );
        }
    }
}

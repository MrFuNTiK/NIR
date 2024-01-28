#include <stdlib.h>
#include <math.h>
#include <loop_unrolling.h>
#include "sliding_fft.h"

typedef enum
{
    SIN_TABLE,
    COS_TABLE,
    NUM_TABLES
} PRECALC_TABLES;

struct SlidingFft_st
{
    double _Complex* spectrum;
    double* real;
    double* twiddleCoef[ NUM_TABLES ];
    double lastSample;

    size_t numSamples;
    size_t realIdx;
    size_t samplesProcessed;
};

SlidingFFT* SlidingFFT_create( size_t numSamples )
{
    SlidingFFT* tf= malloc( sizeof( SlidingFFT ) );
    if( tf )
    {
        tf->real = ( double* )malloc( sizeof( double ) * numSamples );
        tf->twiddleCoef[ SIN_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );
        tf->twiddleCoef[ COS_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );
        tf->spectrum = ( double _Complex* )malloc( sizeof( double _Complex ) * ( numSamples / 2 + 1 ) );

        if( !tf->real ||
            !tf->twiddleCoef[ SIN_TABLE ] ||
            !tf->twiddleCoef[ COS_TABLE ] ||
            !tf->spectrum )
        {
            goto err;
        }

        for( size_t i = 0; i < numSamples; ++i )
        {
            tf->real[ i ] = 0;
            if( i < numSamples / 2 + 1 )
            {
                double omega = 2 * M_PI * i / numSamples;
                if( isnan( omega ) )
                {
                    goto err;
                }
                tf->twiddleCoef[ SIN_TABLE ][ i ] = sin( omega );
                tf->twiddleCoef[ COS_TABLE ][ i ] = cos( omega );
                tf->spectrum[ i ] = 0 + 0*I;
            }
        }

        tf->numSamples = numSamples;
        tf->samplesProcessed = 0;
        tf->realIdx = 0;
        tf->lastSample = 0.;
    }

    return tf;

err:
    SlidingFFT_destroy( tf );
    return NULL;
}

int SlidingFFT_update( SlidingFFT* tf, double sample )
{
    double prev;

    if( !tf )
    {
        return 0;
    }

    prev = tf->real[ tf->realIdx ];
    tf->real[ tf->realIdx ] = sample;

#ifdef ENABLE_LOOP_UNROLLING
    UNROLL_LOOP( UNROLL_FACTOR_EIGHT, i, 0, tf->numSamples / 2 + 1,
        tf->spectrum[ i ] = tf->twiddleCoef[ COS_TABLE ][ i ] * ( tf->spectrum[ i ] - prev + sample ) +
                            tf->twiddleCoef[ SIN_TABLE ][ i ] * ( tf->spectrum[ i ] - prev + sample ) * I;
    )
#else
    for( size_t i = 0; i < tf->numSamples / 2 + 1; ++i )
    {
        tf->spectrum[ i ] = tf->twiddleCoef[ COS_TABLE ][ i ] * ( tf->spectrum[ i ] - prev + sample ) +
                            tf->twiddleCoef[ SIN_TABLE ][ i ] * ( tf->spectrum[ i ] - prev + sample ) * I;
    }
#endif // ENABLE_LOOP_UNROLLING

    size_t newIdx = ( ++( tf->realIdx ) ) % tf->numSamples;
    tf->realIdx = newIdx;
    if( tf->samplesProcessed < tf->numSamples )
    {
        ++tf->samplesProcessed;
    }

    return 1;
}

double _Complex* SlidingFFT_GetSpectrum( SlidingFFT* tf )
{
    if( !tf )
    {
        return NULL;
    }

    if( tf->samplesProcessed < tf->numSamples )
    {
        return NULL;
    }

    return tf->spectrum;
}

void SlidingFFT_destroy( SlidingFFT* tf )
{
    if( tf )
    {
        free( tf->real );
        free( tf->twiddleCoef[ SIN_TABLE ] );
        free( tf->twiddleCoef[ COS_TABLE ] );
        free( tf->spectrum );
        free( tf );
    }
}

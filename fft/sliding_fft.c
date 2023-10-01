#include <stdlib.h>
#include <math.h>
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
    double* twiddleCoef[ NUM_TABLES ];
    double lastSample;

    size_t numSamples;
    size_t samplesProcessed;
};

SlidingFFT* SlidingFFT_create( size_t numSamples )
{
    SlidingFFT* tf= malloc( sizeof( SlidingFFT ) );
    if( tf )
    {
        tf->twiddleCoef[ SIN_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );
        tf->twiddleCoef[ COS_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );
        tf->spectrum = ( double _Complex* )malloc( sizeof( double _Complex ) * ( numSamples / 2 + 1 ) ) ;

        if( !tf->twiddleCoef[ SIN_TABLE ] ||
            !tf->twiddleCoef[ COS_TABLE ] ||
            !tf->spectrum )
        {
            goto err;
        }

        for( size_t i = 0; i < numSamples / 2 + 1; ++i )
        {
            double omega = 2 * M_PI * i / numSamples;
            if( isnan( omega ) )
            {
                goto err;
            }
            tf->twiddleCoef[ SIN_TABLE ][ i ] = sin( omega );
            tf->twiddleCoef[ COS_TABLE ][ i ] = cos( omega );
        }

        tf->numSamples = numSamples;
        tf->samplesProcessed = 0;
        tf->lastSample = 0.;
    }

    return tf;
err:
    free( tf->twiddleCoef[ SIN_TABLE ] );
    free( tf->twiddleCoef[ COS_TABLE ] );
    free( tf->spectrum );
    free( tf );
    return NULL;
}

int SlidingFFT_update( SlidingFFT* tf, double sample )
{
    if( !tf )
    {
        return 0;
    }

    for( size_t i = 0; i < tf->numSamples; ++i )
    {
        tf->spectrum[ i ] = tf->twiddleCoef[ COS_TABLE ][ i ] * ( tf->spectrum[ i ] - tf->lastSample + sample ) +
                            tf->twiddleCoef[ SIN_TABLE ][ i ] * ( tf->spectrum[ i ] - tf->lastSample + sample ) * I;
    }
    tf->lastSample = sample;
    if( tf->samplesProcessed < tf->numSamples )
    {
        ++tf->samplesProcessed;
    }

    return 1;
}

const double _Complex* SlidingFFT_GetSpectrum( SlidingFFT* tf )
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
        free( tf->spectrum );
        free( tf->twiddleCoef[ SIN_TABLE ] );
        free( tf->twiddleCoef[ COS_TABLE ] );
        free( tf );
    }
}

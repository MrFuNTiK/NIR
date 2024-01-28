#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "goerzel.h"

#define ENABLE_PRECALC

typedef enum
{
    SIN_TABLE,
    COS_TABLE,
    NUM_TABLES
} PRECALC_TABLES;

struct GoerzTf_st
{
    double _Complex res;
    double q0, q1, q2;
    double sin_, cos_;

#ifdef ENABLE_PRECALC
    double* preCalcTable[ NUM_TABLES ];
    size_t firstFreqIndex_, freqIndexWidth_;
#endif

    size_t numSamples;
    size_t updateCount;
};

GoerzelTF* GoerzelTF_create( size_t numSamples )
{
    GoerzelTF* tf = ( GoerzelTF* )malloc( sizeof ( GoerzelTF ) );
    if( tf )
    {
        tf->numSamples = numSamples;
        tf->updateCount = 0ul;
        tf->res = NAN;
        tf->sin_ = NAN;
        tf->cos_ = NAN;
#ifdef ENABLE_PRECALC
        tf->preCalcTable[ SIN_TABLE ] = NULL;
        tf->preCalcTable[ COS_TABLE ] = NULL;
#endif
    }
    return tf;
}

void GoerzelTF_destroy( GoerzelTF* tf )
{
#ifdef ENABLE_PRECALC
    free( tf->preCalcTable[ SIN_TABLE ] );
    free( tf->preCalcTable[ COS_TABLE ] );
#endif
    free( tf );
}

void GoerzelTF_set_freq_idx( GoerzelTF* tf, size_t idx )
{
    assert( tf );

    tf->q0 = 0.0;
    tf->q1 = 0.0;
    tf->q2 = 0.0;
#ifdef ENABLE_PRECALC
    assert( idx >= tf->firstFreqIndex_ );
    assert( idx <= tf->firstFreqIndex_ + tf->freqIndexWidth_ );
    assert( tf->preCalcTable[ SIN_TABLE ] );
    assert( tf->preCalcTable[ COS_TABLE ] );
    tf->sin_ = tf->preCalcTable[ SIN_TABLE ][ idx - tf->firstFreqIndex_ ];
    tf->cos_ = tf->preCalcTable[ COS_TABLE ][ idx - tf->firstFreqIndex_ ];
#else
    assert( idx < tf->numSamples / 2 + 1 );
    tf->sin_ = sin( 2 * M_PI * idx / tf->numSamples );
    tf->cos_ = cos( 2 * M_PI * idx / tf->numSamples );
#endif
    tf->res = NAN;

    assert( !isnan( tf->sin_ ) );
    assert( !isnan( tf->cos_ ) );
}

void GoerzelTF_update( GoerzelTF* tf, double data )
{
    assert( tf );
    ++tf->updateCount;

    tf->q2 = tf->q1;
    tf->q1 = tf->q0;
    tf->q0 = 2 * tf->cos_ * tf->q1 - tf->q2 + data;
}

double _Complex GoerzelTF_result( GoerzelTF* tf )
{
    assert( tf );
    tf->res = ( - tf->q1 + tf->q0 * tf->cos_ ) / ( tf->numSamples ) +
              ( tf->q0 * tf->sin_ )*I / ( tf->numSamples );
    return tf->updateCount > 3 ? tf->res : NAN;
}

size_t GoerzelTF_get_update_count( GoerzelTF* tf )
{
    assert( tf );
    return tf->updateCount;
}

int GoerzelTF_precalc( GoerzelTF* tf, size_t firstIdx, size_t numSamples )
{
#ifdef ENABLE_PRECALC
    assert( tf );
    assert( firstIdx <= tf->numSamples );
    assert( firstIdx + numSamples <= tf->numSamples / 2 + 1 );

    tf->firstFreqIndex_ = firstIdx;
    tf->freqIndexWidth_ = numSamples;

    free( tf->preCalcTable[ SIN_TABLE ] );
    free( tf->preCalcTable[ COS_TABLE ] );
    tf->preCalcTable[ SIN_TABLE ] = ( double* )malloc( sizeof( double ) * numSamples );
    tf->preCalcTable[ COS_TABLE ] = ( double* )malloc( sizeof( double ) * numSamples );

    if( !tf->preCalcTable[ SIN_TABLE ] || !tf->preCalcTable[ COS_TABLE ] )
    {
        return 0;
    }

    for( size_t i = 0; i < tf->freqIndexWidth_; ++i )
    {
        double omega = 2 * M_PI * ( tf->firstFreqIndex_ + i ) / tf->numSamples;
        if( isnan( omega ) )
        {
            return 0;
        }
        tf->preCalcTable[ SIN_TABLE ][ i ] = sin( omega );
        tf->preCalcTable[ COS_TABLE ][ i ] = cos( omega );
    }
    return 1;
#else
    ( void )tf;
    ( void )firstIdx;
    ( void )numSamples;
    return 1;
#endif // ENABLE_PRECALC
}

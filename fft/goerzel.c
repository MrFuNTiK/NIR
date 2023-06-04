#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "goerzel.h"

#define ENABLE_PRECALC

typedef enum
{
    SIN_TABLE = 0,
    COS_TABLE = 1
} PRECALC_TABLES;

struct GoerzTf_st
{
    double _Complex res;
    double q0, q1, q2;
    double sin_, cos_;

#ifdef ENABLE_PRECALC
    double* preCalcTable[ 2 ];
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
#ifdef ENABLE_PRECALC
        tf->preCalcTable[ SIN_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );
        tf->preCalcTable[ COS_TABLE ] = ( double* )malloc( sizeof( double ) * ( numSamples / 2 + 1 ) );

        if( !tf->preCalcTable[ SIN_TABLE ] || !tf->preCalcTable[ COS_TABLE ] )
        {
            GoerzelTF_destroy( tf );
            tf = NULL;
        }

        for( size_t i = 0; i < numSamples / 2 + 1; ++i )
        {
            double omega = 2 * M_PI * i / numSamples;
            tf->preCalcTable[ SIN_TABLE ][ i ] = sin( omega );
            tf->preCalcTable[ COS_TABLE ][ i ] = cos( omega );
            if( isnan( tf->preCalcTable[ SIN_TABLE ][ i ] ) ||
                isnan( tf->preCalcTable[ COS_TABLE ][ i ] ) )
            {
                GoerzelTF_destroy( tf );
                tf = NULL;
            }
        }
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

int GoerzelTF_set_freq_idx( GoerzelTF* tf, size_t idx )
{
    assert( tf );
    assert( idx < tf->numSamples / 2 + 1 );

    tf->q0 = 0.0;
    tf->q1 = 0.0;
    tf->q2 = 0.0;
#ifdef ENABLE_PRECALC
    tf->sin_ = tf->preCalcTable[ SIN_TABLE ][ idx ];
    tf->cos_ = tf->preCalcTable[ COS_TABLE ][ idx ];
#else
    tf->sin_ = sin( 2 * M_PI * idx / tf->numSamples );
    tf->cos_ = cos( 2 * M_PI * idx / tf->numSamples );
#endif
    tf->res = NAN;

    if( isnan( tf->sin_ ) ||
        isnan( tf->cos_ ) )
    {
        return 0;
    }

    return 1;
}

int GoerzelTF_update( GoerzelTF* tf, double data )
{
    assert( tf );
    ++tf->updateCount;

    tf->q2 = tf->q1;
    tf->q1 = tf->q0;
    tf->q0 = 2 * tf->cos_ * tf->q1 - tf->q2 + data;

    return 1;
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

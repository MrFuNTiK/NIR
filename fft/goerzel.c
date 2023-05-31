#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "goerzel.h"

struct GoerzTf_st
{
    double _Complex res;
    double q0, q1, q2;
    double omega_, sin_, cos_;

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
    }
    return tf;
}

void GoerzelTF_destroy( GoerzelTF* tf )
{
    free( tf );
}

int GoerzelTF_set_freq_idx( GoerzelTF* tf, size_t idx )
{
    assert( tf );
    assert( idx < tf->numSamples / 2 + 1 );

    tf->q0 = 0.0;
    tf->q1 = 0.0;
    tf->q2 = 0.0;
    tf->omega_ = 2 * M_PI * idx / tf->numSamples;
    tf->sin_ = sin( tf->omega_ );
    tf->cos_ = cos( tf->omega_ );
    tf->res = NAN;

    if( isnan( tf->omega_ ) ||
        isnan( tf->sin_ ) ||
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

    tf->q0 = 2 * tf->cos_ * tf->q1 - tf->q2 + data;
    tf->q2 = tf->q1;
    tf->q1 = tf->q0;

    return 1;
}

double _Complex GoerzelTF_result( GoerzelTF* tf )
{
    assert( tf );
    tf->res = ( tf->q1 - tf->q2 * tf->cos_ ) / ( tf->numSamples / 2 ) +
              ( tf->q2 * tf->sin_ )*I / ( tf->numSamples / 2 );
    return tf->updateCount > 3 ? tf->res : NAN;
}

size_t GoerzelTF_get_update_count( GoerzelTF* tf )
{
    assert( tf );
    return tf->updateCount;
}

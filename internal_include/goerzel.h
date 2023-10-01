#pragma once

#include <complex.h>

typedef struct GoerzTf_st GoerzelTF;

#ifdef __cplusplus
extern "C" {
#endif // _cplusplus

GoerzelTF* GoerzelTF_create( size_t numSamples );

void GoerzelTF_destroy( GoerzelTF* tf );

void GoerzelTF_set_freq_idx( GoerzelTF* tf, size_t idx );

void GoerzelTF_update( GoerzelTF* tf, double data );

double _Complex GoerzelTF_result( GoerzelTF* tf );

size_t GoerzelTF_get_update_count( GoerzelTF* tf );

int GoerzelTF_precalc( GoerzelTF* tf, size_t firstIdx, size_t numSamples );

#ifdef __cplusplus
}
#endif // _cplusplus

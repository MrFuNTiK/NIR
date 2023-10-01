#pragma once

#include <complex.h>

typedef struct SlidingFft_st SlidingFFT;

SlidingFFT* SlidingFFT_create( size_t numSamples );

int SlidingFFT_update( SlidingFFT* tf, double sample );

const double _Complex* SlidingFFT_GetSpectrum( SlidingFFT* tf );

void SlidingFFT_destroy( SlidingFFT* tf );

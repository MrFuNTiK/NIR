# TDE TOOLS

# Description

Main purpose of these programms is researching and  comparison of GCC(BCC) and GPS(BPS) TDE methods in relations to voice.

# Source code structure
- core - contains so,e specific functions, eg. calculating phase and amplitude spectrums from fourier images.
- fft - object-wrapper for fftw3 library that provides direct and reverse fourier transform.
- correlation - class that implements TDE via GCC(BPS) method.
- phase - class that implements TDE via GPS(BPS) method.
- src - testing of functionality of GCC and GPS.

# Future develop
- PHAT weigting function for GCC
- Coherence function for both GCC and GPS

# Used libraries:
- FFTW3 library: http://fftw.org/
- Header-only library for wav-files: https://github.com/adamstark/AudioFile
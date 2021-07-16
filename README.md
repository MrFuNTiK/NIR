# TDE TOOLS

# Description

Main purpose of these programms is researching and  comparison of GCC(BCC) and GPS(BPS) TDE methods in relations to voice.

# Source code structure
- core - contains some specific functions, eg. calculating GPS and amplitude spectrums from fourier images.
- fft - object-wrapper for fftw3 library that provides direct and reverse fourier transform.
- TDE - parent class with public TDE intarface.
- GCC - child class of TDE that implements GCC(BPS) TDE method.
- GPS - child class of TDE that implements GPS(BPS) TDE method.
- src - testing of functionality of GCC and GPS.

# Future develop
- PHAT weigting function for GCC
- Coherence function for both GCC and GPS

# Used libraries:
- FFTW3 library: http://fftw.org/
- Header-only library for wav-files: https://github.com/adamstark/AudioFile
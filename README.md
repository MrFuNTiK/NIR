# TDE TOOLS

# Description

Main purpose of these programms is researching and  comparison of GCC(BCC) and GPS(BPS) TDE methods in relations to voice.

# Source code structure
- fft - object-wrapper for fftw3 library that provides direct and reverse fourier transform.
- TDE - interface-class TDE and implementations GCC and GPS methods of TDE
- cmd_util - command utility provides functionality to calculate TDE

# Future develop
- Using data from ADC instead of reading file
- GUI to draw graphics for weighting functions, amplitude cross-spectrum, phase cross-spectrum

# Used libraries:
- FFTW3 library: http://fftw.org/
- Header-only library for wav-files: https://github.com/adamstark/AudioFile

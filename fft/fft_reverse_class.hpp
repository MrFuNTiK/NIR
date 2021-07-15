#ifndef FFT_REVERSE_CLASS_HPP
# define FFT_REVERSE_CLASS_HPP

# include <cstdint>
# include <fftw3.h>

class fft_reverse
{
public:
    fft_reverse(uint16_t _size);
    ~fft_reverse();

    void execute();
    void set_fourier_image(fftw_complex* _fourier);
    void get_real(double* _real);

private:
    uint16_t size;
    fftw_plan reverse_plan;
    fftw_complex* fourier_image;
    double* real_array;
};

#endif // FFT_REVERSE_CLASS_HPP
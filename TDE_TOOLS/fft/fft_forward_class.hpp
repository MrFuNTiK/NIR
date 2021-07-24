#ifndef FFT_FORWARD_CLASS_HPP
# define FFT_FORWARD_CLASS_HPP

# include <cstdint>
# include <fftw3.h>

class fft_forward
{
public:
    fft_forward(uint16_t _size);
    ~fft_forward();

    void execute();
    void conjugate();
    void set_real(double* _real);
    void get_fourier_image(fftw_complex* _fourier);

private:
    uint16_t size;
    fftw_plan forward_plan;
    double* real_array;
    fftw_complex* fourier_image;
    void normalize_fur();
};

#endif // FFT_FORWARD_CLASS_HPP
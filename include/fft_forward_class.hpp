///@file fft_forward_class.hpp
///@brief class-wrapper above fftw3 library that implements forward fourier transform

#ifndef FFT_FORWARD_CLASS_HPP
# define FFT_FORWARD_CLASS_HPP

# include <cstdint>
# include <fftw3.h>
# include <memory>

///@defgroup FFT_interface
///@addtogroup FFT_interface
///@{

///@defgroup forward_fourier_transform
///@addtogroup forward_fourier_transform
///@{

/**
 * @class fft_forward
 * @brief This class implements direct Fast Fourier Transform.
 *
 * Size of processed real array should be passed in constructor.
 * Size of output complex array is equal (N / 2 + 1).
 */
class fft_forward
{
public:

    fft_forward() = delete;
    fft_forward(fft_forward*) = delete;
    void operator = (const fft_forward*) = delete;

    /**
     * @brief Construct a new fft forward object
     *
     * @param _size     Number of samples of real input samples.
     *                  Output array will contain (_size / 2 + 1) complex samples.
     */
    fft_forward(uint16_t _size);
    ~fft_forward();

    /**
     * @brief Execute direct transform of arrays passed in set_real().
     */
    void execute() noexcept;

    /**
     * @brief Make complex conjugation of result of transform.
     */
    void conjugate() noexcept;

    /**
     * @brief Pass array that should be processed.
     *
     * Passed array will be copied inside class, so you can safely
     * reuse passed array.
     *
     * @param[in] _real Pointer to array
     */
    void set_real(double* _real) noexcept;

    /**
     * @brief Get the result of direct transform
     *
     * @param[out] _fourier Pointer to array where the result should be copied
     */
    void get_fourier_image(fftw_complex* _fourier) noexcept;

private:
    uint16_t size;
    fftw_plan forward_plan;
    double* real_array;
    fftw_complex* fourier_image;
    void normalize_fur();
};

///@} forward_fourier_transform
///@} FFT_interface

#endif // FFT_FORWARD_CLASS_HPP

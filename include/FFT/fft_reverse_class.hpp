///@file fft_reverse_class.hpp
///@brief class-wrapper above fftw3 library that implements reverse fourier transform

#ifndef FFT_REVERSE_CLASS_HPP
# define FFT_REVERSE_CLASS_HPP

# include <cstdint>
# include <vector>
# include <complex>
# include <fftw3.h>

///@defgroup FFT_interface
///@addtogroup FFT_interface
///@{

///@defgroup reverse_fourier_transform
///@addtogroup reverse_fourier_transform
///@{

namespace transform
{

namespace cpu
{

namespace fft
{

/**
 * @class fft_reverse
 * @brief This class implements reverse Fast Fourier Transform
 */
class Reverse
{
public:
    Reverse() = delete;
    Reverse(Reverse&) = delete;
    void operator = (const Reverse&) = delete;

    /**
     * @brief Construct a new fft reverse object.
     *
     * @param _size     Number of samples of output real array.
     *
     * Note that _size is number of elements in real arrays. So size of input
     * complex array to set_fourier_image() is (_size / 2 + 1) and
     * size of output real array from get_real() is (_size).
     */
    Reverse(uint16_t _size);
    ~Reverse();

    /**
     * @brief Execute reverse transform with array passed via set_fourier_image().
     */
    void Execute() noexcept;

    /**
     * @brief Set the fourier image.
     *
     * Passed array will be copied inside class, so can safely
     * reuse passed array.
     *
     * @param[in] _fourier      Pointer to complex array to process
     */
    void SetFourierImage(const std::vector<std::complex<double>>& _fourier) noexcept;

    /**
     * @brief Get the real array.
     *
     * @param[out] _real        Pointer to array where resault should be written
     */
    void GetReal(std::vector<double>& _real) noexcept;

private:
    uint16_t size;
    fftw_plan reverse_plan;
    std::vector<std::complex<double>> fourier_image;
    std::vector<double> real_array;
};

} // namespace fft

} // namespace cpu

} // namespace transform

///@} reverse_fourier_transform
///@} FFT_interface

#endif // FFT_REVERSE_CLASS_HPP

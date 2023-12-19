///@file fft_reverse_class.hpp
///@brief class-wrapper above fftw3 library that implements reverse fourier transform

#pragma once

#include <FFT/fft_base.hpp>
#include <fftw3.h>

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

namespace reverse
{

/**
 * @class fft_reverse
 * @brief This class implements reverse Fast Fourier Transform
 */
class FFT final : Reverse
{
public:
    FFT() = delete;
    FFT(const FFT&) = delete;
    FFT& operator = (const FFT&) = delete;

    /**
     * @brief Construct a new fft reverse object.
     *
     * @param size     Number of samples of output real array.
     *
     * Note that _size is number of elements in real arrays. So size of input
     * complex array to set_fourier_image() is (size / 2 + 1) and
     * size of output real array from get_real() is (size).
     */
    FFT(size_t size);
    ~FFT();

    /**
     * @brief Execute reverse transform with array passed via set_fourier_image().
     */
    void Execute() override;

    /**
     * @brief Set the fourier image.
     *
     * Passed array will be copied inside class, so can safely
     * reuse passed array.
     *
     * @param[in] fourier      Pointer to complex array to process
     */
    void SetFourierImage(const std::vector<std::complex<double>>& fourier) override;

    /**
     * @brief Get the real array.
     *
     * @param[out] real        Pointer to array where resault should be written
     */
    void GetReal(std::vector<double>& real) override;

private:
    size_t size_;
    fftw_plan reverse_plan_;
    std::vector<std::complex<double>> fourier_image_;
    std::vector<double> real_array_;
};

} // namespace fft

} // namespace cpu

} // namespace transform

///@} reverse_fourier_transform
///@} FFT_interface

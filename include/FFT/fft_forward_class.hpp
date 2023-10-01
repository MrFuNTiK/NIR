///@file fft_forward_class.hpp
///@brief class-wrapper above fftw3 library that implements forward fourier transform

#pragma once

#include <FFT/fft_base.hpp>
#include <fftw3.h>

///@defgroup FFT_interface
///@addtogroup FFT_interface
///@{

///@defgroup forward_fourier_transform
///@addtogroup forward_fourier_transform
///@{

namespace transform
{

namespace cpu
{

namespace forward
{

/**
 * @class Transform
 * @brief This class implements direct Fast Fourier Transform.
 *
 * Size of processed real array should be passed in constructor.
 * Size of output complex array is equal (N / 2 + 1).
 */
class FFT final : Forward
{
public:

    FFT() = delete;
    FFT(const FFT&) = delete;
    FFT& operator = (const Forward&) = delete;

    /**
     * @brief Construct a new fft forward object
     *
     * @param _size     Number of samples of real input samples.
     *                  Output array will contain (_size / 2 + 1) complex samples.
     */
    FFT(size_t _size);
    ~FFT();

    /**
     * @brief Execute direct transform of arrays passed in set_real().
     */
    void Execute() noexcept override;

    /**
     * @brief Make complex conjugation of result of transform.
     */
    void Conjugate() noexcept override;

    /**
     * @brief Pass array that should be processed.
     *
     * Passed array will be copied inside class, so you can safely
     * reuse passed array.
     *
     * @param[in] _real Pointer to array
     */
    void SetReal(const std::vector<double>& _real) noexcept override;

    /**
     * @brief Get the result of direct transform
     *
     * @param[out] _fourier Pointer to array where the result should be copied
     */
    void GetFourierImage(std::vector<std::complex<double>>& _fourier) noexcept override;

private:
    size_t size;
    fftw_plan forward_plan;
    std::vector<double> real_array;
    std::vector<std::complex<double>> fourier_image;
    void NormalizeFur();
};

} // namespace fft

} // namespace cpu

} // namespace transform

///@} forward_fourier_transform
///@} FFT_interface

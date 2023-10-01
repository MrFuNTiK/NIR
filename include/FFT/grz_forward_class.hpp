///@file fft_forward_class.hpp
///@brief class-wrapper above fftw3 library that implements forward fourier transform

#pragma once

#include <FFT/fft_base.hpp>
#include <complex>
#include <memory>
#include <goerzel.h>

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

using GoerzPtr = std::unique_ptr< GoerzelTF, decltype( &GoerzelTF_destroy ) >;

/**
 * @class Transform
 * @brief This class implements direct Fast Fourier Transform.
 *
 * Size of processed real array should be passed in constructor.
 * Size of output complex array is equal (N / 2 + 1).
 */
class Goerzel final : Forward
{
public:
    Goerzel() = delete;
    Goerzel(const Goerzel&) = delete;
    Goerzel& operator = (const Goerzel&) = delete;

    /**
     * @brief Construct a new fft forward object
     *
     * @param size     Number of samples of real input samples.
     */
    Goerzel(size_t size);
    Goerzel(size_t size, size_t lowerBound, size_t upperBound);
    ~Goerzel();

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
    size_t size = 0;
    size_t lowerBound_ = 0;
    size_t upperBound_ = 0;
    GoerzPtr goerzHandle{ nullptr, GoerzelTF_destroy };
    std::vector<double> real_array;
    std::vector<std::complex<double>> fourier_image;
    void NormalizeFur();
};

} // namespace grz

} // namespace cpu

} // namespace transform

///@} forward_fourier_transform

///@} FFT_interface

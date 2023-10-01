///@file fft_base.hpp
///@brief Base classes for all implementations of direct and reverse Fourier transforms.

#pragma once

# include <vector>
# include <complex>

///@defgroup FFT_interface
///@addtogroup FFT_interface
///@{

///@defgroup forward_fourier_transform
///@addtogroup forward_fourier_transform
///@{

namespace transform
{

/**
 * @class Transform
 * @brief This class implements direct Fast Fourier Transform.
 *
 * Size of processed real array should be passed in constructor.
 * Size of output complex array is equal (N / 2 + 1).
 */
class Forward
{
public:

    Forward() = default;
    Forward(const Forward&) = delete;
    Forward& operator = (const Forward&) = delete;

    /**
     * @brief Construct a new fft forward object
     *
     * @param size     Number of samples of real input samples.
     *                 Output array will contain (size / 2 + 1) complex samples.
     */
    Forward(size_t size);
    virtual ~Forward() = default;

    /**
     * @brief Execute direct transform of arrays passed in set_real().
     */
    virtual void Execute() noexcept = 0;

    /**
     * @brief Make complex conjugation of result of transform.
     */
    virtual void Conjugate() noexcept = 0;

    /**
     * @brief Pass array that should be processed.
     *
     * Passed array will be copied inside class, so you can safely
     * reuse passed array.
     *
     * @param[in] real Pointer to array
     */
    virtual void SetReal(const std::vector<double>& real) noexcept = 0;

    /**
     * @brief Get the result of direct transform
     *
     * @param[out] fourier Resized vector to copy result to.
     */
    virtual void GetFourierImage(std::vector<std::complex<double>>& fourier) noexcept = 0;
};

} // namespace transform

///@} forward_fourier_transform





///@addtogroup reverse_fourier_transform
///@{

namespace transform
{

/**
 * @class fft_reverse
 * @brief This class implements reverse Fast Fourier Transform
 */
class Reverse
{
public:
    Reverse() = default;
    Reverse(const Reverse&) = delete;
    Reverse& operator = (const Reverse&) = delete;

    /**
     * @brief Construct a new fft reverse object.
     *
     * @param size     Number of samples of output real array.
     *
     * Note that _size is number of elements in real arrays. So size of input
     * complex array to set_fourier_image() is (size / 2 + 1) and
     * size of output real array from get_real() is (size).
     */
    Reverse(size_t size);
    virtual ~Reverse() = default;

    /**
     * @brief Execute reverse transform with array passed via set_fourier_image().
     */
    virtual void Execute() noexcept = 0;

    /**
     * @brief Set the fourier image.
     *
     * Passed array will be copied inside class, so can safely
     * reuse passed array.
     *
     * @param[in] fourier      Complex vector to process.
     */
    virtual void SetFourierImage(const std::vector<std::complex<double>>& fourier) noexcept = 0;

    /**
     * @brief Get the real array.
     *
     * @param[out] _real        Pointer to array where resault should be written
     */
    virtual void GetReal(std::vector<double>& _real) noexcept = 0;
};

} // namespace transform

///@} reverse_fourier_transform
///@} FFT_interface

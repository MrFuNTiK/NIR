///@file GCC_class.hpp
///@brief Implementation of generalized cross correlation method of time delay estimation.

#ifndef CORRELATION_METHOD_CLASS_HPP
# define CORRELATION_METHOD_CLASS_HPP

# include <cstdint>

# include "TDE_class.hpp"
# include <FFT/fft_forward_class.hpp>
# include <FFT/fft_reverse_class.hpp>

using namespace transform::cpu::fft;

///@addtogroup TDE_interface
///@{

///@defgroup GCC_TDE
///@addtogroup GCC_TDE
///@{

namespace tde
{

namespace gcc
{

/**
 * @class GCC
 * @brief This class implements calculation of TDE with generalized cross correlation method.
 */
class GCC final : public iTDE
{
public:
    GCC() = delete;
    GCC(GCC*) = delete;
    void operator = (const GCC*) = delete;
    GCC(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func);
    ~GCC();

    void GetCorrFunc(std::vector<double>& _corr);
    void Update(const std::vector<double>& first_, const std::vector<double>& second_) override;
    void Conclude() override;

private:
    std::vector<double> corr_func;
    std::vector<double> PHAT_func;

private:
    Forward forward;
    Reverse reverse;
    void shift_corr_func();
    void apply_PHAT_func(double* weight_func);
};

} // namespace gcc

} // namespce tde

///@} GCC_TDE
///@} TDE_interface

#endif // CORRELATION_METHOD_CLASS_HPP

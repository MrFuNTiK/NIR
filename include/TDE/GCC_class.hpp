///@file GCC_class.hpp
///@brief Implementation of generalized cross correlation method of time delay estimation.

#ifndef CORRELATION_METHOD_CLASS_HPP
# define CORRELATION_METHOD_CLASS_HPP

# include <cstdint>

# include "TDE_class.hpp"
# include <FFT/fft_forward_class.hpp>
# include <FFT/fft_reverse_class.hpp>

///@addtogroup TDE_interface
///@{

///@defgroup GCC_TDE
///@addtogroup GCC_TDE
///@{

/**
 * @class GCC
 * @brief This class implements calculation of TDE with generalized cross correlation method.
 */
class GCC final : public TDE_calc
{
public:
    GCC() = delete;
    GCC(GCC*) = delete;
    void operator = (const GCC*) = delete;
    GCC(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    ~GCC();

    void get_corr_func(std::vector<double>& _corr);
    void update(const std::vector<double>& first_, const std::vector<double>& second_) override;
    void conclude() override;

private:
    std::vector<double> corr_func;
    std::vector<double> PHAT_func;

private:
    fft_forward forward;
    fft_reverse reverse;
    void shift_corr_func();
    void apply_PHAT_func(double* weight_func);
};

///@} GCC_TDE
///@} TDE_interface

#endif // CORRELATION_METHOD_CLASS_HPP

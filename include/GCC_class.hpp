#ifndef CORRELATION_CLASS_HPP
# define CORRELATION_CLASS_HPP

# include <cstdint>

# include <TDE_class.hpp>
# include <fft_forward_class.hpp>
# include <fft_reverse_class.hpp>


class GCC final : public TDE
{
public:
    GCC() = delete;
    GCC(GCC*) = delete;
    void operator = (const GCC*) = delete;
    GCC(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    ~GCC();

    void get_corr_func(double* _corr);
    void update(double* first_, double* second_) override;
    void conclude() override;

private:
    double* corr_func;
    double* PHAT_func;

private:
    fft_forward forward;
    fft_reverse reverse;
    void shift_corr_func();
    void apply_PHAT_func(double* weight_func);
};

#endif // CORRELATION_CLASS_HPP

#ifndef CORRELATION_CLASS_HPP
# define CORRELATION_CLASS_HPP

# include <fftw3.h>
# include <cstdint>

# include "TDE_class.hpp"


class GCC : public TDE
{
public:
    GCC(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate);
    ~GCC();

    void get_corr_func(double* _corr);
    void execute() override;
    void calculate_tde() override;

private:
    double* corr_func;
    double* PHAT_func;

private:
    void shift_corr_func();
    void apply_PHAT_func(double* weight_func);
};

#endif // CORRELATION_CLASS_HPP
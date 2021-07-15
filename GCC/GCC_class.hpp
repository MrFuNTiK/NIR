#ifndef CORRELATION_CLASS_HPP
# define CORRELATION_CLASS_HPP

# include <fftw3.h>
# include <cstdint>

//#include "tde_class.hpp"
//#include "../tde/tde_class.hpp"
//#include "../tde/tde_class.hpp"


class GCC //: public tde_class
{
public:
    GCC(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate);
    ~GCC();

    void set_arrays(double** _first, double** _second);
    void get_corr_func(double* _corr);
    void execute();
    void calculate_tde();
    double get_tde();

private:
    uint16_t size;
    uint16_t avrg_win_num;
    uint16_t sample_rate;
    double tde;
    double** first_array;
    double** second_array;
    double* corr_func;

    fftw_complex* fur_1;
    fftw_complex* fur_2;
    fftw_complex* fur_1_2;
    fftw_complex* fur_1_2_sum;

private:
    void get_mul();
    void clear_sum();
    void add_mul_to_sum();
    void normalize_sum();
    void shift_corr_func();
};

#endif // CORRELATION_CLASS_HPP
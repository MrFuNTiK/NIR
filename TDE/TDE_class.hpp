#ifndef TDE_CLASS_HPP
# define TDE_CLASS_HPP

# include <cstdint>
# include <fftw3.h>

class TDE
{
public:
    TDE(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate);
    virtual ~TDE();

    void set_arrays(double** _first, double** _second);
    virtual void execute() = 0;
    virtual void calculate_tde() = 0;
    double get_tde();

protected:
    uint16_t avrg_win_num;
    uint16_t size;
    uint16_t sample_rate;
    double tde;
    double** first_array;
    double** second_array;

    fftw_complex* fur_1;
    fftw_complex* fur_2;
    fftw_complex* fur_1_2;
    fftw_complex* fur_1_2_sum;

protected:
    void get_mul();
    void clear_sum();
    void add_mul_to_sum();
    void normalize_sum();
    virtual void apply_weight_func(double* weight_func) = 0;
};

#endif // TDE_CLASS_HPP
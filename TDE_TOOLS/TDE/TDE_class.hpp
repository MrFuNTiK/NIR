#ifndef TDE_CLASS_HPP
# define TDE_CLASS_HPP

# include <cstdint>
# include <fftw3.h>

class TDE
{
public:
    TDE(uint16_t _size, uint16_t _rate);
    virtual ~TDE();

    virtual void update(double* _first, double* _second) = 0;
    virtual void conclude() = 0;
    double get_tde();

protected:
    uint16_t size;
    uint16_t sample_rate;
    uint16_t update_count;
    double tde;

    fftw_complex* fur_1;
    fftw_complex* fur_2;
    fftw_complex* fur_1_2;
    fftw_complex* fur_1_2_sum;

protected:
    void get_mul();
    void clear_sum();
    void add_mul_to_sum();
    void normalize_sum();
};

#endif // TDE_CLASS_HPP

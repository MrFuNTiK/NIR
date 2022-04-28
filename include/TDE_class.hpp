#ifndef TDE_CLASS_HPP
# define TDE_CLASS_HPP

# include <cstdint>
# include <fftw3.h>

typedef enum
{
    GCC_TDE,
    GPS_TDE
} tde_meth;
typedef enum
{
    COHERENCE,
    NONE
} weighting_func;

class TDE
{
public:
    TDE() = delete;
    TDE(TDE*) = delete;
    void operator = (const TDE*) = delete;
    TDE(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    virtual ~TDE();

    virtual void update(double* _first, double* _second) = 0;
    virtual void conclude() = 0;
    double get_tde();

protected:
    uint16_t size;
    uint16_t sample_rate;
    uint16_t update_count;
    weighting_func w_func;
    double tde;

    fftw_complex* fur_1;
    fftw_complex* fur_2;
    fftw_complex* fur_1_2;
    fftw_complex* fur_1_2_sum;

    double* ampl1;
    double* ampl2;
    double* ampl1_sum;
    double* ampl2_sum;

protected:
    void make_mul();
    void make_mul_with_conj();
    void clear_inner();
    void add_mul_to_sum();
    void normalize_sum();
};

#endif // TDE_CLASS_HPP

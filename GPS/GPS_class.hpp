#ifndef CORRELATION_PHASE_CLASS_H
#define CORRELATION_PHASE_CLASS_H

#include <fftw3.h>
#include <cstdint>

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

class GPS
{
public:
    GPS(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate);
    ~GPS();

    void set_arrays(double** _first, double** _second);
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

    fftw_complex* fur_1;
    fftw_complex* fur_2;
    fftw_complex* fur_1_2;
    fftw_complex* fur_1_2_sum;

    double* cross_phase_spectrum;

    void get_mul();
    void clear_sum();
    void add_mul_to_sum();
    void normalize_sum();
};

#endif //CORRELATION_PHASE_CLASS_H

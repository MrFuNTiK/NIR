#ifndef CORRELATION_PHASE_CLASS_H
#define CORRELATION_PHASE_CLASS_H

#include <fftw3.h>
#include <cstdint>

#include "TDE_class.hpp"

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

class GPS : public TDE
{
public:
    GPS(uint16_t _avrg_win_num, uint16_t _size, uint16_t _rate);
    ~GPS();

    void execute() override;
    void calculate_tde() override;

private:
    double* cross_phase_spectrum;
};

#endif //CORRELATION_PHASE_CLASS_H

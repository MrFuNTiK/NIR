#ifndef CORRELATION_PHASE_CLASS_H
#define CORRELATION_PHASE_CLASS_H

#include <cstdint>

#include <TDE_class.hpp>
#include <fft_forward_class.hpp>

#define BOTTOM_FREQ_BOUND   300
#define UPPER_FREQ_BOUND    3400

class GPS final : public TDE
{
public:
    GPS() = delete;
    GPS(GPS*) = delete;
    void operator = (const GPS*) = delete;
    GPS(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    ~GPS();

    void update(double* _first, double* _second) override;
    void conclude() override;

private:
    fft_forward forward;
    double* cross_phase_spectrum;
};

#endif //CORRELATION_PHASE_CLASS_H

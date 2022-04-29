///@file GPS_class.hpp
///@brief Implementation of generalized phase spectrum method of time delay estimation.

#ifndef CORRELATION_PHASE_CLASS_H
#define CORRELATION_PHASE_CLASS_H

#include <cstdint>

#include "TDE_class.hpp"
#include <FFT/fft_forward_class.hpp>

///@addtogroup TDE_interface
///@{

///@defgroup GPS_TDE
///@addtogroup GPS_TDE
///@{

///@defgroup frequency_bounds
///@addtogroup frequency_bounds
///@{

#define BOTTOM_FREQ_BOUND   300     /// Frequencies lower this value will not be accounted
#define UPPER_FREQ_BOUND    3400    /// Frequencies upper this value will not be accounted

///@} frequency_bounds

/**
 * @class GPS
 * @brief This class implements calculation of TDE with generalized phase spectrum method.
 */
class GPS final : public TDE_calc
{
public:
    GPS() = delete;
    GPS(GPS*) = delete;
    void operator = (const GPS*) = delete;
    GPS(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    ~GPS();

    void update(double* _first, double* _second) noexcept override;
    void conclude() noexcept override;

private:
    fft_forward forward;
    double* cross_phase_spectrum;
};

///@} GPS_TDE
///@} TDE_interface

#endif //CORRELATION_PHASE_CLASS_H

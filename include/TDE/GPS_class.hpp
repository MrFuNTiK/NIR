///@file GPS_class.hpp
///@brief Implementation of generalized phase spectrum method of time delay estimation.

#pragma once

#include "TDE_class.hpp"
#include <FFT/fft_forward_class.hpp>

using namespace transform::cpu;

///@addtogroup TDE_interface
///@{

///@defgroup GPS_TDE
///@addtogroup GPS_TDE
///@{

namespace tde
{

namespace gps
{

/**
 * @class GPS
 * @brief This class implements calculation of TDE with generalized phase spectrum method.
 */
class GPS final : public iTDE
{
public:
    GPS() = delete;
    GPS(const GPS&) = delete;
    void operator = (const GPS&) = delete;
    GPS(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func);
    ~GPS();

    void Update(const std::vector<double>& _first, const std::vector<double>& _second) override;
    void Conclude() override;

private:
    forward::FFT forward;
    std::vector<double> cross_phase_spectrum;
};

} // namespace gps

} // namespace tde

///@} GPS_TDE
///@} TDE_interface

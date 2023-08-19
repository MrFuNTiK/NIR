///@file GPS_class.hpp
///@brief Implementation of generalized phase spectrum method of time delay estimation.

#pragma once

#include <vector>
#include <cstdint>

#include "TDE_class.hpp"
#include <FFT/grz_forward_class.hpp>

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
 * @class GPS_GRZ
 * @brief This class implements calculation of TDE with generalized phase spectrum method
 *        with use of Goerzel transform to calsculate signals spectrum.
 */
class GPS_GRZ final : public iTDE
{
public:
    GPS_GRZ() = delete;
    GPS_GRZ(GPS_GRZ&) = delete;
    void operator = (const GPS_GRZ&) = delete;
    GPS_GRZ(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func);
    ~GPS_GRZ();

    void Update(const std::vector<double>& _first, const std::vector<double>& _second) override;
    void Conclude() override;

private:
    size_t lowerBound;
    size_t upperBound;
    grz::Forward forward;
    std::vector<double> cross_phase_spectrum;
};

} // namespace gps

} // namespace tde

///@} GPS_TDE
///@} TDE_interface

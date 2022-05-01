/// @file TDE_class.hpp
/// @brief Header file for interface class TDE

#ifndef TDE_CLASS_HPP
# define TDE_CLASS_HPP

# include <cstdint>
# include <vector>
# include <complex>
# include <fftw3.h>

///@defgroup TDE_interface
///@addtogroup TDE_interface
///@{

/**
 * @brief Avaliable methods to calculate TDE corresponding @ref GCC "GCC" and @ref GPS "GPS" classes.
 */
typedef enum
{
    GCC_TDE,
    GPS_TDE,
} tde_meth;

/**
 * @brief Avaliable frequency-weighting functions.
 */
typedef enum
{
    COHERENCE,
    NONE,
} weighting_func;

/**
 * @class TDE_calc
 * @brief Class for TDE calculation.
 */
class TDE_calc
{
public:
    TDE_calc() = delete;
    TDE_calc(TDE_calc*) = delete;
    void operator = (const TDE_calc*) = delete;

    /**
     * @brief Construct a new TDE object.
     *
     * @throws std::exception   in case of invalid parameters in constructor
     *
     * @param _size     Number source signal's samples in one window
     * @param _rate     Sampling frequency of source signal
     * @param _w_func   @ref weighting_func "Frequency-weighting" function used to calculate TDE
     */
    TDE_calc(uint16_t _size, uint16_t _rate, weighting_func _w_func);
    virtual ~TDE_calc();

    /**
     * @brief Process one window of two channels of source signal.
     *
     * Method calculates and accumulates intermediate spectums and other data
     * according to implementation in inherited class. Can be called
     * several times before conclude() with different input windows to avergage calculations
     * by several windows. In case of use weighting functions must be calles several times.
     *
     * @param _first    Pointer to the first channel samples
     * @param _second   Pointer to the second channel samples
     */
    virtual void update(double* _first, double* _second)  noexcept = 0;

    /**
     * @brief Calculate TDE.
     *
     * Method calculates TDE using data accumulated by calls of update().
     * Method cleans all inner data used to calculate TDE, so next call of this method
     * will calculate TDE value using data, stored by calls of update() after
     * previous call of conclude().
     */
    virtual void conclude() noexcept = 0;

    /**
     * @brief Get value of calculated TDE.
     *
     * @return TDE value
     */
    double get_tde();

    ///@} // TDE_interface

protected:
    uint16_t size;
    uint16_t sample_rate;
    uint16_t update_count;
    weighting_func w_func;
    double tde;

    std::vector<std::complex<double>> fur_1;
    std::vector<std::complex<double>> fur_2;
    std::vector<std::complex<double>> fur_1_2;
    std::vector<std::complex<double>> fur_1_2_sum;

    std::vector<double> ampl1;
    std::vector<double> ampl2;
    std::vector<double> ampl1_sum;
    std::vector<double> ampl2_sum;

protected:
    void make_mul();
    void make_mul_with_conj();
    void clear_inner();
    void add_mul_to_sum();
    void normalize_sum();
};

#endif // TDE_CLASS_HPP

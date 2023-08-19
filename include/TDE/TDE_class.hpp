/// @file TDE_class.hpp
/// @brief Header file for interface class TDE

#ifndef TDE_CLASS_HPP
# define TDE_CLASS_HPP

# include <cstdint>
# include <vector>
# include <complex>

///@defgroup TDE_interface
///@addtogroup TDE_interface
///@{

namespace tde
{

/**
 * @brief Avaliable frequency-weighting functions.
 */
typedef enum
{
    COHERENCE,  ///< Use coherence frequency weighting function
    NONE,       ///< No frequency weighting function used
} WEIGHTING_FN_TYPE;

static constexpr char COHERENCE_WEIGHT_FN_STR[] = "coherence";
static constexpr char NONE_WEIGHTING_FN_STR[] = "none";

std::string WEIGHTING_FN_TYPE_to_str( WEIGHTING_FN_TYPE type );

WEIGHTING_FN_TYPE WEIGHTING_FN_TYPE_from_str( std::string& str );

/**
 * @class TDE_calc
 * @brief Class for TDE calculation.
 */
class iTDE
{
public:
    iTDE() = delete;
    iTDE(iTDE*) = delete;
    void operator = (const iTDE*) = delete;

    /**
     * @brief Construct a new TDE object.
     *
     * @throws std::exception   in case of invalid parameters in constructor
     *
     * @param _size     Number source signal's samples in one window
     * @param _rate     Sampling frequency of source signal
     * @param _w_func   @ref WEIGHTING_FN_TYPE "Frequency-weighting" function used to calculate TDE
     */
    iTDE(size_t _size, size_t _rate, WEIGHTING_FN_TYPE _w_func);
    virtual ~iTDE();

    /**
     * @brief Process one window of two channels of source signal.
     *
     * Method calculates and accumulates intermediate spectums and other data
     * according to implementation in inherited class. Can be called
     * several times before Conclude() with different input windows to avergage calculations
     * by several windows. In case of use weighting functions must be calles several times.
     *
     * @param _first    Pointer to the first channel samples
     * @param _second   Pointer to the second channel samples
     */
    virtual void Update(const std::vector<double>& _first, const std::vector<double>& _second) = 0;

    /**
     * @brief Calculate TDE.
     *
     * Method calculates TDE using data accumulated by calls of update().
     * Method cleans all inner data used to calculate TDE, so next call of this method
     * will calculate TDE value using data, stored by calls of update() after
     * previous call of conclude().
     */
    virtual void Conclude() = 0;

    /**
     * @brief Get value of calculated TDE.
     *
     * @return TDE value
     */
    double GetTde();

    ///@} // TDE_interface

protected:
    size_t size;
    size_t sample_rate;
    WEIGHTING_FN_TYPE w_func;
    size_t update_count;
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

} // namespace tde

#endif // TDE_CLASS_HPP

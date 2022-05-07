#ifndef PROGRAMM_ENVORINMENT_HPP
# define PROGRAMM_ENVORINMENT_HPP

# include <memory>

# include <TDE/TDE_class.hpp>

///@defgroup utility_helpers
///@addtogroup utility_helpers
///@{

///@defgroup program_environment
///@addtogroup program_environment
///@{

/**
 * @brief Avaliable methods to calculate TDE
 */
typedef enum
{
    GCC_TDE,    ///< @ref program_environment::CreateCalculator() "CreateCalculator()" creates object of GCC
    GPS_TDE,    ///< @ref program_environment::CreateCalculator() "CreateCalculator()" creates object of GPS
} tde_meth;

/**
 * @class program_environment
 * @brief This class implements singleton manager of utility execution.
 */
class program_environment
{
private:
    static std::shared_ptr<program_environment> _pe;
    uint16_t _window_size;
    uint16_t _sample_rate;
    uint16_t _window_avrg_num;
    tde_meth _meth;
    weighting_func _weight_fn;
    bool _isExecutable;
    program_environment();

public:
    ~program_environment();
    program_environment(program_environment* pe) = delete;
    void operator = (const program_environment*) = delete;

    /**
     * @brief Get the Instance object
     *
     * @return std::shared_ptr<program_environment> instance of manager object
     */
    static std::shared_ptr<program_environment> GetInstance();

    /**
     * @brief Set the size of windows to process.
     *
     * @param window_size
     */
    void SetWindowSize(uint16_t window_size);

    /**
     * @brief Set the sampling frequency of processed signals.
     *
     * @param sample_rate
     */
    void SetSampleRate(uint16_t sample_rate);

    /**
     * @brief Set the window averaging number.
     *
     * @param num
     */
    void SetWinAvrgNum(uint16_t num);

    /**
     * @brief Set the TDE method to use.
     *
     * @param meth
     */
    void SetMethodTDE(tde_meth meth);

    /**
     * @brief Set the frequency weighting function to use.
     *
     * @param feighting_fn
     */
    void SetWeightingFunction(weighting_func feighting_fn);

    /**
     * @brief Set executable status.
     *
     * In order to enable further execution method should be called after all parameters were set.
     *
     * Note that program_environment constructor sets up SIGINT interrupt handler
     * that sets executable value to false in order to correct terminate program.
     *
     * @param executable
     */
    void SetExecutable(bool executable);

    /**
     * @brief Create a TDE calculator.
     *
     * Created object is implementation of method set in SetMethodTDE().
     * Returned object is created with parameters passed by SetWindowSize(),
     * SetSampleRate(), SetWeightingFunction().
     *
     * @throws std::exception   in case of invalid parameters (actually thrown by calculator constructor)
     *
     * @return TDE_calc*
     */
    TDE_calc* CreateCalculator();

    /**
     * @brief Get value of window size parameter.
     *
     * @return uint16_t
     */
    uint16_t GetWindowSize();

    /**
     * @brief Get value of sampling frequency parameter.
     *
     * @return uint16_t
     */
    uint16_t GetSampleRate();

    /**
     * @brief Get value of window averaging number parameter.
     *
     * @return uint16_t
     */
    uint16_t GetWinAvrgNum();

    /**
     * @brief Check wheather the execution is enabled.
     *
     * State can be changed explicitly by call of SetExecutable() (true and false) and
     * implicitly by SIGINT interrupt handler (only false).
     *
     * @return true     if execution enabled
     * @return false    if execution disabled
     */
    bool isExecutable();
};

///@} program_environment
///@} utility_helpers

#endif // PROGRAMM_ENVORINMENT_HPP

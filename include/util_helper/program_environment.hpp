#ifndef PROGRAMM_ENVORINMENT_HPP
# define PROGRAMM_ENVORINMENT_HPP

# include <TDE/TDE_class.hpp>
# include <logger/logger.hpp>

///@defgroup utility_helpers
///@addtogroup utility_helpers
///@{

///@defgroup program_environment
///@addtogroup program_environment
///@{

/**
 * @brief Avaliable methods to calculate TDE
 */

namespace utility_helpers
{

namespace program_environment
{

typedef enum
{
    GCC_TDE,    ///< @ref program_environment::CreateCalculator() "CreateCalculator()" creates object of GCC
    GPS_TDE,    ///< @ref program_environment::CreateCalculator() "CreateCalculator()" creates object of GPS
} TDE_METH;

static constexpr char TDE_METHOD_GCC_STR[] = "GCC";
static constexpr char TDE_METHOD_GPS_STR[] = "GPS";

std::string TDE_METH_to_str( TDE_METH meth );

TDE_METH TDE_METH_from_str( std::string& str );

namespace defaults
{

static const TDE_METH TDE_METHOD = GPS_TDE;
static const uint16_t SAMPLE_RATE = 44100;
static const uint16_t WINDOW_SIZE = 1 << 10;
static const uint16_t AVRG_NUM = 1;
static const tde::WEIGHTING_FN_TYPE WEIGHTING_FN = tde::NONE;
#ifdef ENABLE_LOGGER
static const logger::EVENTS LOG_EVENTS = logger::EVENTS::SOUND |
                                         logger::EVENTS::MANAGE |
                                         logger::EVENTS::CREATE;
static constexpr char LOG_PATH[] = "logfile.txt";
#endif // ENABLE_LOGGER

} // namespace defaults

namespace cmd_params
{

static const size_t MAX_PARAM_LENGHT =  128;

static constexpr char TDE_METHOD[] =      "tde-method";
static constexpr char SAMPLE_RATE[] =   "sample-rate";
static constexpr char WINDOWS_SIZE[] =  "win-size";
static constexpr char AVRG_NUM[] =      "avrg-num";
static constexpr char WEIGHTING_FN[] =  "weight-fn";
static constexpr char LOG_EVENTS[] =    "log-events";
static constexpr char LOG_PATH[] =      "log-path";

}

/**
 * @class program_environment
 * @brief This class implements singleton manager of utility execution.
 */
class ProgramEnvironment
{
private:
    uint16_t _window_size;
    uint16_t _sample_rate;
    uint16_t _window_avrg_num;
    TDE_METH _meth;
    tde::WEIGHTING_FN_TYPE _weight_fn;
    bool _isExecutable;
    ProgramEnvironment();
    ~ProgramEnvironment();

public:
    ProgramEnvironment(ProgramEnvironment* pe) = delete;
    void operator = (const ProgramEnvironment*) = delete;

    /**
     * @brief Get the Instance object
     *
     * @return program_environment* instance of manager object
     */
    static ProgramEnvironment* GetInstance();

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
    void SetMethodTDE(TDE_METH meth);

    /**
     * @brief Set the frequency weighting function to use.
     *
     * @param feighting_fn
     */
    void SetWeightingFunction(tde::WEIGHTING_FN_TYPE weighting_fn);

# ifdef ENABLE_LOGGER
    /**
     * @brief Set kinds of events to write to logs.
     *
     * @param events
     */
    void SetLogEvents(logger::EVENTS events);

    /**
     * @brief Set path to logger.
     *
     * @param path
     */
    void SetLogPath(std::string& path);
# endif // ENABLE_LOGGER

    /**
     * @brief Set up environment by cmd arguments
     *
     * @param argc
     * @param argv
     */
    void SetUpByArgs(int argc, char** argv);

    /**
     * @brief
     *
     * @param progName
     */
    void PrintHelp(const char* progName);

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
    tde::iTDE* CreateCalculator();

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

} // namespace programm_environment

} // namespace utility_helpers

///@} program_environment
///@} utility_helpers

#endif // PROGRAMM_ENVORINMENT_HPP

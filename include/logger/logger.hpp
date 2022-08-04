/// @file logger.hpp
/// @brief Logger class declaration

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <mutex>
# include <memory>
# include <cstdint>

///@defgroup utility_helpers
///@addtogroup utility_helpers
///@{

///@defgroup Logger
///@addtogroup Logger
///@{

/**
 * @brief Avaliable event types to log.
 */
enum class EVENTS : int
{
    CREATE = 1,     ///< Resource managing e.g. create or destroy object.
    SOUND = 2,      ///< Actions with sound e.g. copy data from RtAudio buffer.
    MANAGE = 4,     ///< Change behaviour of cmd_util e.g. by calling SIGINT handler.
    TDE_CALC = 8,   ///< Any math calculations.
    ERROR = 16      ///< Error occurred.
};

EVENTS operator | (EVENTS ev1, EVENTS ev2);
EVENTS operator & (EVENTS ev1, EVENTS ev2);
bool operator ! (EVENTS ev);

/**
 * @brief Thread-safe logger singleton
 */
class logger
{
public:
    /**
     * @brief Get the Instance object.
     *
     * @return std::shared_ptr<logger> Instance of logger.
     */
    static std::shared_ptr<logger> GetInstance();

    /**
     * @brief Set target trace path.
     *
     * Method set path of output file to write logs, If method
     * was not called, than logs will be printed to stdout.
     * Takes no effect after logger has been initialized.
     *
     * @param trace Target trace path.
     */
    void SetTrace( const char* trace );

    /**
     * @brief Set event types to log. Events of types which
     * was not set are not being logged.
     * Takes no effect after logger has been initialized.
     *
     * @param eventTypes Bitwice "or" of EVENTS.
     */
    void SetEvents( EVENTS eventTypes );

    /**
     * @brief Initialize logger instance.
     *
     * Tries to open std::ofstream with path given in SetTrace().
     * Double initialization is prohibited.
     *
     * @throws std::exception In case of failure while opening
     *                        ofstream or double initialization.
     */
    void Initialize();

    /**
     * @brief Thread-safe method to log event.
     *
     * Method make log note with given data. Method must not
     * be called directly but inside TRACE_EVENT() macro.
     * Logger must be initialized before call of this method.
     *
     * @param event Type of current event.
     * @param file Current file where event occurred.
     * @param line Current line where event occurred.
     * @param message Message that should be written to note.
     *
     * @throws std::runtime_error In case of logger was not initialized.
     */
    void LogMessage( EVENTS event, const char* file, const int line, const char* message );
    ~logger();

private:
    static std::shared_ptr<logger> _logger;
    logger();
    bool _isInitialized;
    std::mutex _traceMutex;
    std::string _trace;
    std::ostream* _trace_stream;
    EVENTS _events;
    const char* EventToString( EVENTS event );
};

/**
 * \def TRACE_EVENT( EVENT, MESSAGE )
 *
 * Macro invokes instance of logger and log message by call of
 * @ref logger::LogMessage() "LogMessage()".
 *
 * @param EVENT Type of current event.
 * @param MESSAGE Message that should be written to note.
 */

# if defined( ENABLE_LOGGER )
#  define TRACE_EVENT( EVENT, MESSAGE )  logger::GetInstance()->LogMessage( EVENT, \
                                         __FILE__, __LINE__, MESSAGE )
# else
#  define TRACE_EVENT( EVENT, MESSAGE ) (void)0
# endif

///@} Logger
///@} utility_helpers

#endif // LOGGER_HPP

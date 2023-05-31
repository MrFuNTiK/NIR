/// @file logger.hpp
/// @brief Logger class declaration

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <mutex>
# include <cstdint>

///@defgroup utility_helpers
///@addtogroup utility_helpers
///@{

///@defgroup Logger
///@addtogroup Logger
///@{

namespace logger
{

/**
 * @brief Avaliable event types to log.
 */
enum class EVENTS : int
{
    NONE = 0,       ///< Do not log any event.
    CREATE = 1,     ///< Resource managing e.g. create or destroy object.
    SOUND = 2,      ///< Actions with sound e.g. copy data from RtAudio buffer.
    MANAGE = 4,     ///< Change behaviour of cmd_util e.g. by calling SIGINT handler.
    TDE_CALC = 8,   ///< Any math calculations.
    ERROR = 16,     ///< Error occurred.
    DEBUG = 32      ///< Some debug message.
};

static constexpr char NONE_STR[] =      "NONE";
static constexpr char CREATE_STR[] =    "CREATE";
static constexpr char SOUND_STR[] =     "SOUND";
static constexpr char MANAGE_STR[] =    "MANAGE";
static constexpr char TDE_CALC_STR[] =  "TDE_CALC";
static constexpr char ERROR_STR[] =     "ERROR";
static constexpr char DEBUG_STR[] =     "DEBUG";

EVENTS operator | (EVENTS ev1, EVENTS ev2);
EVENTS operator & (EVENTS ev1, EVENTS ev2);
bool operator ! (EVENTS ev);

std::string EVENTS_to_str( EVENTS event );

EVENTS EVENTS_from_str( std::string& str );

/**
 * @brief Thread-safe logger singleton
 */
class Logger
{
public:
    Logger(Logger&) = delete;
    Logger operator =(Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger operator =(Logger&&) = delete;
    ~Logger();


    /**
     * @brief Get the Instance object.
     *
     * @return logger* Instance of logger.
     */
    static Logger& GetInstance();

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
     * @param func Function name.
     * @param message Message that should be written to note.
     *
     * @throws std::runtime_error In case of logger was not initialized.
     */
    void LogMessage( EVENTS event, const char* func, const char* message );

private:
    Logger();
    bool _isInitialized;
    std::mutex _traceMutex;
    std::string _trace;
    std::ostream* _traceStream;
    std::FILE* _traceFile;
    bool needCloseFile;
    EVENTS _events;
    void CreateStream( std::string path );
    void CloseStream();
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
#  define TRACE_EVENT( EVENT, MESSAGE )    \
    {                                      \
        auto& lg = logger::Logger::GetInstance(); \
        lg.LogMessage( EVENT, __PRETTY_FUNCTION__, MESSAGE ); \
    }
# else
#  define TRACE_EVENT( EVENT, MESSAGE )
# endif

EVENTS EVENTS_parse_events_str( std::string& events );

std::string EVENTS_parse_events( EVENTS events );

} // namespace logger

///@} Logger
///@} utility_helpers

#endif // LOGGER_HPP

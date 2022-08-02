#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <mutex>
# include <memory>
# include <cstdint>

enum class EVENTS : int
{
    CREATE = 1,
    SOUND = 2,
    MANAGE = 4,
    TDE_CALC = 8,
    ERROR = 16
};

EVENTS operator | (EVENTS ev1, EVENTS ev2);
EVENTS operator & (EVENTS ev1, EVENTS ev2);
bool operator ! (EVENTS ev);

class logger
{
public:
    static std::shared_ptr<logger> GetInstance();
    void SetTrace( const char* trace );
    void SetEvents( EVENTS eventTypes );
    void Initialize();
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

# if defined( ENABLE_LOGGER )
#  define TRACE_EVENT( EVENT, MESSAGE )  logger::GetInstance()->LogMessage( EVENT, \
                                         __FILE__, __LINE__, MESSAGE )
# else
#  define TRACE_EVENT( EVENT, MESSAGE ) (void)0
# endif

#endif // LOGGER_HPP

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <mutex>
# include <memory>
# include <cstdint>

enum class EVENTS : int
{
    EVENT = 1,
    ERROR = 2
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

# define TRACE_EVENT( EVENT, MESSAGE )  logger::GetInstance()->LogMessage( EVENT, \
                                        __FILE__, __LINE__, MESSAGE )

#endif // LOGGER_HPP

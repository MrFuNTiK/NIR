#include <fstream>
#include <iostream>
#include <logger/logger.hpp>

std::shared_ptr<logger> logger::GetInstance()
{
    if( !_logger.get() )
    {
        _logger.reset(new logger);
    }
    return _logger;
}

logger::logger() :
    _isInitialized(false),
    _trace_stream(nullptr)
{}

logger::~logger()
{
    if( !_isInitialized )
    {
        return;
    }

    if( _trace_stream != &std::cout )
    {
        delete _trace_stream;
    }
}

void logger::SetTrace( const char* trace )
{
    if( !_isInitialized ) _trace = trace;
}

void logger::SetEvents( EVENTS eventTypes )
{
    if( !_isInitialized ) _events = eventTypes;
}

void logger::Initialize()
{
    if( _isInitialized )
    {
        throw std::logic_error( "Logger is already initialized" );
    }

    if( !_trace.empty() )
    {
        _trace_stream = new std::ofstream( _trace );
        _isInitialized = true;
        return;
    }

    _trace_stream = &std::cout;
    _isInitialized = true;
}

void logger::LogMessage( EVENTS event, const char* file, const int line, const char* message )
{
    if( !( _events & event ) )
    {
        return;
    }

    if( !_isInitialized )
    {
        throw::std::runtime_error( "Attempt to use uninitialized looger" );
    }

    const char* eventStr = EventToString( event );
    std::lock_guard<std::mutex> lg( _traceMutex );
    *_trace_stream << eventStr << ":\t" << file << ":" << line <<
                      "\t" << message <<std::endl;
}

const char* logger::EventToString( EVENTS event )
{
    const char* str;
    switch (event)
    {
    case EVENTS::CREATE:
    {
        str = "CREATE";
        break;
    }
    case EVENTS::SOUND:
    {
        str = "SOUND";
        break;
    }
    case EVENTS::MANAGE:
    {
        str = "MANAGE";
        break;
    }
    case EVENTS::TDE_CALC:
    {
        str = "TDE_CALC";
        break;
    }
    case EVENTS::ERROR:
    {
        str = "ERROR";
        break;
    }
    default:
        throw std::runtime_error("Unrecognized event type");
    }

    return str;
}

EVENTS operator | (EVENTS ev1, EVENTS ev2)
{
    return static_cast<EVENTS>( static_cast<std::underlying_type<EVENTS>::type>(ev1) | static_cast<std::underlying_type<EVENTS>::type>(ev2) );
}

EVENTS operator & (EVENTS ev1, EVENTS ev2)
{
    return static_cast<EVENTS>( static_cast<std::underlying_type<EVENTS>::type>(ev1) & static_cast<std::underlying_type<EVENTS>::type>(ev2) );
}

bool operator ! (EVENTS ev)
{
    return ( 0 == static_cast<std::underlying_type<EVENTS>::type>(ev) );
}

std::shared_ptr<logger> logger::_logger( nullptr );

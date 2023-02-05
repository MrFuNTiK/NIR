#include <fstream>
#include <sstream>
#include <iostream>
#include <logger/logger.hpp>

namespace logger{

Logger* Logger::GetInstance()
{
    static Logger _logger;
    return &_logger;
}

Logger::Logger() :
    _isInitialized(false),
    _traceStream(nullptr)
{}

Logger::~Logger()
{
    if( !_isInitialized )
    {
        return;
    }

    CloseStream();
}

void Logger::SetTrace( const char* trace )
{
    if( !_isInitialized ) _trace = trace;
}

void Logger::SetEvents( EVENTS eventTypes )
{
    if( !_isInitialized ) _events = eventTypes;
}

void Logger::Initialize()
{
    if( _isInitialized )
    {
        throw std::logic_error( "Logger is already initialized" );
    }

    /*
    if( !_trace.empty() )
    {
        _traceStream = new std::ofstream( _trace );
        _isInitialized = true;
        return;
    }

    _traceStream = &std::cout;
    _isInitialized = true;
    */

    CreateStream(_trace);
    _isInitialized = true;
}

void Logger::LogMessage( EVENTS event, const char* file, const int line, const char* message )
{
    if( !( _events & event ) )
    {
        return;
    }

    if( !_isInitialized )
    {
        throw::std::runtime_error( "Attempt to use uninitialized logger" );
    }

    std::string eventStr = EVENTS_to_str( event );
    std::lock_guard<std::mutex> lg( _traceMutex );

    *_traceStream << eventStr.c_str() << ":\t" << file << ":"
                  << line << "\t" << message <<std::endl;
}

void Logger::CreateStream( std::string path )
{
    if( path == "stdout" )
    {
        _traceStream = &std::cout;
        needCloseFile = false;
    }
    else
    {
        _traceStream = new std::ofstream(path);
        needCloseFile = true;
    }
}

void Logger::CloseStream()
{
    if( needCloseFile )
    {
        delete _traceStream;
        _traceStream = nullptr;
    }
}

std::string EVENTS_to_str( EVENTS event )
{
    switch (event)
    {
    case EVENTS::NONE:
    {
        return NONE_STR;
    }
    case EVENTS::CREATE:
    {
        return CREATE_STR;
    }
    case EVENTS::SOUND:
    {
        return SOUND_STR;
    }
    case EVENTS::MANAGE:
    {
        return MANAGE_STR;
    }
    case EVENTS::TDE_CALC:
    {
        return TDE_CALC_STR;
    }
    case EVENTS::ERROR:
    {
        return ERROR_STR;
    }
    default:
        throw std::runtime_error("Unrecognized event type");
    }
}

EVENTS EVENTS_from_str( std::string& str )
{
    if( str.empty() )
    {
        throw std::runtime_error( "Empty string" );
    }

    if( str == "NONE" )
    {
        return EVENTS::NONE;
    }
    else if( str == CREATE_STR )
    {
        return EVENTS::CREATE;
    }
    else if( str == SOUND_STR )
    {
        return EVENTS::SOUND;
    }
    else if( str == MANAGE_STR )
    {
        return EVENTS::MANAGE;
    }
    else if( str == TDE_CALC_STR )
    {
        return EVENTS::TDE_CALC;
    }
    else if( str == ERROR_STR )
    {
        return EVENTS::ERROR;
    }
    else
    {
        std::stringstream error;
        error << "Undefined event in string: " << str;
        throw std::runtime_error( error.str() );
    }
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

EVENTS EVENTS_parse_events_str( std::string& eventStr )
{
    if( eventStr.empty() )
    {
        throw std::runtime_error( "Event string is empty" );
    }

    auto events = EVENTS::NONE;

    if( std::string::npos != eventStr.find( CREATE_STR ) )
    {
        events = events | EVENTS::CREATE;
    }
    if( std::string::npos != eventStr.find( SOUND_STR ) )
    {
        events = events | EVENTS::SOUND;
    }
    if( std::string::npos != eventStr.find( MANAGE_STR ) )
    {
        events = events | EVENTS::MANAGE;
    }
    if( std::string::npos != eventStr.find( TDE_CALC_STR ) )
    {
        events = events | EVENTS::TDE_CALC;
    }
    if( std::string::npos != eventStr.find( ERROR_STR ) )
    {
        events = events | EVENTS::ERROR;
    }
    return events;
}

std::string EVENTS_parse_events( EVENTS events )
{
    std::stringstream eventStr;
    if( EVENTS::NONE == events )
    {
        return NONE_STR;
    }
    if( EVENTS::NONE != ( EVENTS::CREATE & events ) )
    {
        eventStr << CREATE_STR <<  "|";
    }
    if( EVENTS::NONE != ( EVENTS::ERROR & events ) )
    {
        eventStr << ERROR_STR <<  "|";
    }
    if( EVENTS::NONE != ( EVENTS::MANAGE & events ) )
    {
        eventStr << MANAGE_STR <<  "|";
    }
    if( EVENTS::NONE != ( EVENTS::SOUND & events ) )
    {
        eventStr << SOUND_STR <<  "|";
    }
    if( EVENTS::NONE != ( EVENTS::TDE_CALC & events ) )
    {
        eventStr << TDE_CALC_STR <<  "|";
    }
    eventStr << "\b";
    return eventStr.str();
}

} // namespace logger

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
    _trace = trace;
}

void logger::SetEvents( EVENTS eventTypes )
{
    _events = eventTypes;
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
    case EVENT:
    {
        str = "EVENT";
        break;
    }
    case ERROR:
    {
        str = "ERROR";
        break;
    }
    default:
        throw std::runtime_error("Unrecognized event type");
    }

    return str;
}

std::shared_ptr<logger> logger::_logger( nullptr );

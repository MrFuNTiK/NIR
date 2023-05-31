#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <util_helper/program_environment.hpp>
#include <logger/logger.hpp>
#include <TDE/GCC_class.hpp>
#include <TDE/GPS_class.hpp>
#include <TDE/GPS_GRZ_class.hpp>

#include <util_helper/sigint.hpp>

using namespace tde;
using namespace logger;

namespace utility_helpers::program_environment
{

namespace cmd_params
{
const char paramPrefix[] = "--";
}

void SIGINT_handler(int)
{
    std::cout << "\b\b";
    TRACE_EVENT(EVENTS::MANAGE, "SIGINT recieved");
    ProgramEnvironment::GetInstance().SetExecutable(false);
}

ProgramEnvironment::ProgramEnvironment() :
    _window_size(defaults::WINDOW_SIZE),
    _sample_rate(defaults::SAMPLE_RATE),
    _window_avrg_num(defaults::AVRG_NUM),
    _meth(defaults::TDE_METHOD),
    _weight_fn(defaults::WEIGHTING_FN),
    _isExecutable(false)
{
    signal(SIGINT, SIGINT_handler);
#ifdef ENABLE_LOGGER
    auto& logger = logger::Logger::GetInstance();
    logger.SetEvents(defaults::LOG_EVENTS);
    logger.SetTrace(defaults::LOG_PATH);
#endif // ENABLE_LOGGER
}

ProgramEnvironment::~ProgramEnvironment()
{}

ProgramEnvironment& ProgramEnvironment::GetInstance()
{
    static ProgramEnvironment _pe;
    return _pe;
}

void ProgramEnvironment::SetWindowSize(uint16_t window_size)
{
    _window_size = window_size;
}

void ProgramEnvironment::SetSampleRate(uint16_t sample_rate)
{
    _sample_rate = sample_rate;
}

void ProgramEnvironment::SetWinAvrgNum(uint16_t num)
{
    if(!num)
    {
        throw std::logic_error("Windows avergaging number must be greater than 0");
    }
    _window_avrg_num = num;
}

void ProgramEnvironment::SetMethodTDE(TDE_METH meth)
{
    _meth = meth;
}

void ProgramEnvironment::SetWeightingFunction(tde::WEIGHTING_FN_TYPE weighting_fn)
{
    _weight_fn = weighting_fn;
}

#ifdef ENABLE_LOGGER
void ProgramEnvironment::SetLogPath(std::string& path)
{
    auto& logger = logger::Logger::GetInstance();
    logger.SetTrace(path.c_str());
}

void ProgramEnvironment::SetLogEvents(logger::EVENTS events)
{
    auto& logger = logger::Logger::GetInstance();
    logger.SetEvents(events);
}
#endif // ENABLE_LOGGER

void ProgramEnvironment::SetUpByArgs(int argc, char** argv)
{
    if( argc < 2 )
    {
        return;
    }

    const char* progName = *argv;
    std::string help;

    --argc;
    ++argv;

    help = *argv;
    if( help == "-h" || help == "--help" )
    {
        PrintHelp( progName );
        exit(0);
    }

    if( argc % 2 )
    {
        throw std::runtime_error( "Last parameter value is missing" );
    }

    while( argc > 0 )
    {
        std::string arg = *argv;
        std::string value = *(argv+1);
        if( std::string::npos == arg.find( cmd_params::paramPrefix ) )
        {
            throw std::runtime_error( "Error while parsing arguments" );
        }
        std::string parameter = arg.substr( arg.find( cmd_params::paramPrefix ) + strlen( cmd_params::paramPrefix ) );
        if( parameter.empty() )
        {
            throw std::runtime_error( "Error while parsing arguments" );
        }

        if( parameter == cmd_params::TDE_METHOD )
        {
            SetMethodTDE( TDE_METH_from_str( value ) );
        }
        else if( parameter == cmd_params::SAMPLE_RATE )
        {
            SetSampleRate( static_cast<uint16_t>( std::stoi( value ) ) );
        }
        else if( parameter == cmd_params::WINDOWS_SIZE )
        {
            SetWindowSize( static_cast<uint16_t>( std::stoi( value ) ) );
        }
        else if( parameter == cmd_params::AVRG_NUM )
        {
            SetWinAvrgNum( static_cast<uint16_t>( std::stoi( value ) ) );
        }
        else if( parameter == cmd_params::WEIGHTING_FN )
        {
            SetWeightingFunction( WEIGHTING_FN_TYPE_from_str( value ) );
        }
#ifdef ENABLE_LOGGER
        else if( parameter == cmd_params::LOG_EVENTS )
        {
            SetLogEvents( EVENTS_parse_events_str( value ) );
        }
        else if( parameter == cmd_params::LOG_PATH )
        {
            SetLogPath( value );
        }
#endif // ENABLE_LOGGER
        else
        {
            std::cout << "Incorrect usage" << std::endl;
            PrintHelp( progName );
            exit(0);
        }

        argc -= 2;
        argv += 2;
    }
}

void ProgramEnvironment::PrintHelp(const char* progName)
{
    std::cout << "Usage:" << std::endl;
    std::cout << progName << " "
              << cmd_params::paramPrefix << cmd_params::TDE_METHOD <<   " [value] "
              << cmd_params::paramPrefix << cmd_params::SAMPLE_RATE <<  " [value] "
              << cmd_params::paramPrefix << cmd_params::WINDOWS_SIZE << " [value] "
              << cmd_params::paramPrefix << cmd_params::AVRG_NUM <<     " [value] "
              << cmd_params::paramPrefix << cmd_params::WEIGHTING_FN << " [value] "
#ifdef ENABLE_LOGGER
              << cmd_params::paramPrefix << cmd_params::LOG_EVENTS <<   " [value] "
              << cmd_params::paramPrefix << cmd_params::LOG_PATH <<     " [value] "
#endif // ENABLE_LOGGER
              << std::endl << std::endl;

    std::cout << "Arguments:" << std::endl;
    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::TDE_METHOD
              << "\t" << "[" << TDE_METHOD_GCC_STR << "|" << TDE_METHOD_GPS_STR << "]"
              << "\t\t" << "Method of TDE calculation (default: "
              << TDE_METH_to_str( defaults::TDE_METHOD ) << ")" << std::endl;

    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::SAMPLE_RATE
              << "\t" << "[value]"
              << "\t\t\t" << "Sampling frequency of source signals (default: "
              << defaults::SAMPLE_RATE << ")" << std::endl;

    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::WINDOWS_SIZE
              << "\t" << "[value]"
              << "\t\t\t" << "Number of samples per one window (default: "
              << defaults::WINDOW_SIZE << ")" << std::endl;

    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::AVRG_NUM
              << "\t" << "[value]"
              << "\t\t\t" << "Number of windows for spectrum averaging (default: "
              << defaults::AVRG_NUM << ")" << std::endl;

    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::WEIGHTING_FN
              << "\t" << "[" << tde::NONE_WEIGHTING_FN_STR << "|" << tde::COHERENCE_WEIGHT_FN_STR << "]"
              << "\t" << "Frequency-weighting function (default: "
              << tde::WEIGHTING_FN_TYPE_to_str( defaults::WEIGHTING_FN ) << ")" << std::endl;
#ifdef ENABLE_LOGGER
    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::LOG_EVENTS
              << "\t" << "[value]"
              << "\t\t\t" << "Types of events that should be logged (default: "
              << logger::EVENTS_parse_events( defaults::LOG_EVENTS ) << ")" << std::endl;

    std::cout << "\t" << cmd_params::paramPrefix << cmd_params::LOG_PATH
              << "\t" << "[stdout|value]"
              << "\t\t" << "Path to file to write logs (default: \"{CWD}/" << defaults::LOG_PATH << "\""
              << std::endl << std::endl << std::endl;

    std::cout << "Avaliable events for logging:" << std::endl;
    std::cout << "\t" << logger::NONE_STR << "\t\t" <<      "- disable logging" << std::endl
              << "\t" << logger::CREATE_STR << "\t\t" <<    "- constructing and destructing objects" << std::endl
              << "\t" << logger::ERROR_STR << "\t\t" <<     "- critical errors" << std::endl
              << "\t" << logger::MANAGE_STR << "\t\t" <<    "- enabling/disabling furter execution" << std::endl
              << "\t" << logger::TDE_CALC_STR << "\t" <<    "- calculating TDE" << std::endl;
#endif // ENABLE_LOGGER
}

void ProgramEnvironment::SetExecutable(bool executable)
{
    _isExecutable = executable;
}

iTDE* ProgramEnvironment::CreateCalculator()
{
    if(_sample_rate == 0)
    {
        throw std::runtime_error("Sample rate was not set");
    }
    if(_window_size == 0)
    {
        throw std::runtime_error("Windows size was not set");
    }
    if(NONE != _weight_fn && _window_avrg_num == 1)
    {
        throw std::logic_error("Avergaging number must be greater than 1 to apply frequncy-weighting function");
    }

    if( _meth == GCC_TDE )
    {
        return new tde::gcc::GCC(_window_size, _sample_rate, _weight_fn);
    }
    else if( _meth == GPS_TDE )
    {
        return new tde::gps::GPS(_window_size, _sample_rate, _weight_fn);
    }
    else if( _meth == GPS_GRZ_TDE )
    {
        return new tde::gps::GPS_GRZ(_window_size, _sample_rate, _weight_fn);
    }
    else return nullptr;
}

uint16_t ProgramEnvironment::GetWindowSize()
{
    return _window_size;
}

uint16_t ProgramEnvironment::GetSampleRate()
{
    return _sample_rate;
}

uint16_t ProgramEnvironment::GetWinAvrgNum()
{
    return _window_avrg_num;
}

bool ProgramEnvironment::isExecutable()
{
    return _isExecutable;
}

std::string TDE_METH_to_str( TDE_METH meth )
{
    switch( meth )
    {
    case TDE_METH::GCC_TDE:
    {
        return TDE_METHOD_GCC_STR;
    }
    case TDE_METH::GPS_TDE:
    {
        return TDE_METHOD_GPS_STR;
    }
    case TDE_METH::GPS_GRZ_TDE:
    {
        return TDE_METHOD_GPS_GRZ_STR;
    }
    default:
    {
        throw std::runtime_error( "Undefined TDE method type" );
    }
    }
}

TDE_METH TDE_METH_from_str( std::string& str )
{
    if( str.empty() )
    {
        throw std::runtime_error( "String is empty" );
    }

    if( str == TDE_METHOD_GCC_STR )
    {
        return TDE_METH::GCC_TDE;
    }
    else if( str == TDE_METHOD_GPS_STR )
    {
        return TDE_METH::GPS_TDE;
    }
    else if( str == TDE_METHOD_GPS_GRZ_STR )
    {
        return TDE_METH::GPS_GRZ_TDE;
    }
    else
    {
        std::stringstream error;
        error << "Undefined TDE method: " << str;
        throw std::runtime_error( error.str() );
    }
}

} // namespace program_environment

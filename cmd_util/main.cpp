#include <cstdint>
#include <iostream>
#include <memory>

#include "AudioFile.h"
#include "main.hpp"
#include "TDE_class.hpp"
#include <program_environment.hpp>

static const tde_meth DEFAULT_TDE_METHOD = GPS_TDE;
static const uint16_t DEFAULT_SAMPLE_RATE = 44100;
static const uint16_t DEFAULT_WINDOW_SIZE = 1 << 10;
static const uint16_t DEFAULT_AVRG_NUM = 1;
static const weighting_func DEFAULT_WEIGHTING_FN = NONE;

static void SetUpEnvironmentByArgs(int argc, char* argv[]);

static inline int ArgMatches(const char* argument, const char* value);

static void PrintHelp(const char* progName);

int main(int argc, char* argv[])
{
    double tde = 0;

    std::unique_ptr<TDE_calc> tde_calc;
    std::shared_ptr<program_environment> pe = program_environment::GetInstance();
    if(argc > 1)
    {
        try
        {
            SetUpEnvironmentByArgs(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            PrintHelp(*argv);
            return 0;
        }
    }
    else
    {
        try
        {
            pe->SetMethodTDE(DEFAULT_TDE_METHOD);
            pe->SetWindowSize(DEFAULT_WINDOW_SIZE);
            pe->SetSampleRate(DEFAULT_SAMPLE_RATE);
            pe->SetWinAvrgNum(DEFAULT_AVRG_NUM);
            pe->SetWeightingFunction(DEFAULT_WEIGHTING_FN);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 0;
        }
    }

    try
    {
        tde_calc.reset(pe->CreateCalculator());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
    }

    std::unique_ptr<double> first_array(new double[pe->GetWindowSize()]);
    std::unique_ptr<double> second_array(new double[pe->GetWindowSize()]);

    AudioFile<double> file;
    file.load(WAV_FILE_PATH);
    uint16_t RATE = file.getSampleRate();
    uint8_t num_channels = file.getNumChannels();
    uint16_t window_size = pe->GetWindowSize();
    uint16_t avrg_num = pe->GetWinAvrgNum();

    for( int i = 0; i < 5; ++i)
    {
        for (uint16_t window_num = 3; window_num < avrg_num + 3; ++window_num)
        {
            for (uint16_t sample_num = 0; sample_num < window_size; ++sample_num)
            {
                first_array.get()[sample_num]       = file.samples[0][(i+1)*window_num*window_size + sample_num + SAMPLE_DELAY];
                second_array.get()[sample_num]      = file.samples[0][(i+1)*window_num*window_size + sample_num];
            }
            tde_calc->update(first_array.get(), second_array.get());
        }

        tde_calc->conclude();
        tde = tde_calc->get_tde();
        std::cout << "TDE:    " << tde << std::endl;
    }
}

static void SetUpEnvironmentByArgs(int argc, char* argv[])
{
    std::shared_ptr<program_environment> pe = program_environment::GetInstance();

    const char* progName = *argv;
    --argc;
    ++argv;

    while(argc)
    {
        if(ArgMatches("-tde_method", *argv))
        {
            if(ArgMatches("GCC", *(argv+1)))
            {
                pe->SetMethodTDE(GCC_TDE);
            }
            else if(ArgMatches("GPS",*(argv+1)))
            {
                pe->SetMethodTDE(GPS_TDE);
            }
            else
            {
                throw std::runtime_error("Bad value for tde calculation method");
            }
        }
        else if(ArgMatches("-sample_rate", *argv))
        {
            uint16_t value;
            try
            {
                value = static_cast<uint16_t>(std::stoi(*(argv+1)));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                throw std::runtime_error("Failed get sampling frequency value");
            }
            pe->SetSampleRate(value);
        }
        else if(ArgMatches("-window_size", *argv))
        {
            uint16_t value;
            try
            {
                value = static_cast<uint16_t>(std::stoi(*(argv+1)));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                throw std::runtime_error("Failed get window size value");
            }
            pe->SetWindowSize(value);
        }
        else if(ArgMatches("-avrg_num", *argv))
        {
            uint16_t value;
            try
            {
                value = static_cast<uint16_t>(std::stoi(*(argv+1)));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                throw std::runtime_error("Failed get window size value");
            }
            pe->SetWinAvrgNum(value);
        }
        else if(ArgMatches("-weighting_fn", *argv))
        {
            if(ArgMatches("coherence", *(argv+1)))
            {
                pe->SetWeightingFunction(COHERENCE);
            }
            else
            {
                throw std::runtime_error("Bad value for frequency weighting function");
            }
        }
        else if(ArgMatches("-help", *argv) || ArgMatches("-h", *argv))
        {
            PrintHelp(progName);
            exit(0);
        }
        else
        {
            throw std::runtime_error("Incorrect usage");
        }
        argv += 2;
        argc -= 2;
    }
}

static inline int ArgMatches(const char* argument, const char* value)
{
    return std::strcmp(argument, value) ? 0 : 1;
}

static void PrintHelp(const char* progName)
{
    std::cout << "Usage:" << std::endl;
    std::cout << progName << " " << "-tde_metod [value] -sample_rate [value] "
                                    "-window_size [value] -weighting_fn [value] "
                                    "-avrg_num [value] "
                          << std::endl << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "\t" << "-tde_method"  << "\t" << "[GCC|GPS(default)]" << "\t\t"       << "Method of TDE calculation" << std::endl
              << "\t" << "-sample_rate" << "\t" << "[value]" << "\t\t\t\t"              << "Frequence of source signals (Hz)" << std::endl
              << "\t" << "-window_size" << "\t" << "[value]" << "\t\t\t\t"              << "Number signal's samples in one window" << std::endl
              << "\t" << "-weigting_fn" << "\t" << "[coherence|none(default)]" << "\t"  << "Frequency-wieghting function" << std::endl
              << "\t" << "-avrg_num"    << "\t" << "[value(default - 1)]" << "\t\t"     << "Number of windows to avergage spectrums" << std::endl
              << "\t" << "-help|-h" << "\t\t\t\t\t" << "Print help message" << std::endl;
}

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <filesystem>
#include <unistd.h>

#ifdef ENABLE_WAV_FILE_READING
# include "AudioFile.h"
#endif

#include "main.hpp"
#include "TDE/TDE_class.hpp"
#include <util_helper/program_environment.hpp>
#include <util_helper/sound_provider.hpp>
#include <logger/logger.hpp>

using namespace tde;
using namespace logger;
using namespace utility_helpers::program_environment;
using namespace utility_helpers::sound_provider;

int main(int argc, char* argv[])
{
    double tde = 0;

    std::unique_ptr<iTDE> tde_calc;
    auto& pe = ProgramEnvironment::GetInstance();

    if(argc > 1)
    {
        try
        {
            ProgramEnvironment::GetInstance().SetUpByArgs(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            ProgramEnvironment::GetInstance().PrintHelp(*argv);
            return 0;
        }
    }

#ifdef ENABLE_LOGGER
    try
    {
        auto& logger = logger::Logger::GetInstance();
        logger.Initialize();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
#endif

    try
    {
        tde_calc.reset(pe.CreateCalculator());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
    }

    pe.SetExecutable(true);

    std::vector<double> first_array(pe.GetWindowSize());
    std::vector<double> second_array(pe.GetWindowSize());

#ifdef ENABLE_WAV_FILE_READING
    AudioFile<double> file;
    file.load(WAV_FILE_PATH);
    [[maybe_unused]] uint16_t RATE = file.getSampleRate();
    [[maybe_unused]] uint8_t num_channels = file.getNumChannels();
#endif

    uint16_t avrg_num = pe.GetWinAvrgNum();

    SoundProvider provider(pe.GetSampleRate(), pe.GetWindowSize());

    while(pe.isExecutable()) // will be set to false by SIGINT handler
    {
        for( uint16_t j = 0; j < avrg_num; ++j)
        {
            provider.GetData(first_array, second_array);
            tde_calc->Update(first_array, second_array);
        }

        tde_calc->Conclude();
        tde = tde_calc->GetTde();
        std::cout << "TDE:    " << tde << std::endl;
    }

    return 0;
}

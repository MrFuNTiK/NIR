#include <cstdint>
#include <iostream>
#include <memory>

#include "AudioFile.h"
#include "main.hpp"
#include "TDE_class.hpp"
#include "program_environment/program_environment.hpp"

#define TDE_MODE    GCC_TDE

int main()
{
    uint16_t sample_rate = 44100;
    uint16_t avrg_window_num = 5;
    uint16_t size = 1 << 10;
    double tde = 0;
    double true_delay = SAMPLE_DELAY / static_cast<double>(sample_rate);

    std::unique_ptr<TDE> tde_calc;
    std::shared_ptr<programm_environment> pe(programm_environment::GetInstance());
    pe->SetMethodTDE(GCC_TDE);
    pe->SetWindowSize(size);
    pe->SetSampleRate(sample_rate);
    pe->SetWeightingFunction(COHERENCE);

    try
    {
        tde_calc.reset(pe->GetCalculator());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
    }


    double first_array[size];
    double second_array[size];

    AudioFile<double> file;
    file.load(WAV_FILE_PATH);
    uint16_t RATE = file.getSampleRate();
    uint8_t num_channels = file.getNumChannels();

    for( int i = 0; i < 5; ++i)
    {
        for (uint16_t window_num = 3; window_num < avrg_window_num + 3; ++window_num)
        {
            for (uint16_t sample_num = 0; sample_num < size; ++sample_num)
            {
                first_array[sample_num]       = file.samples[0][(i+1)*window_num*size + sample_num + SAMPLE_DELAY];
                second_array[sample_num]      = file.samples[0][(i+1)*window_num*size + sample_num];
            }
            tde_calc->update(first_array, second_array);
        }

        tde_calc->conclude();
        tde = tde_calc->get_tde();
        std::cout << "TDE:    " << tde << std::endl;
    }
}

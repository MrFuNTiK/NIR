#include <cstdint>
#include <iostream>

#include "AudioFile.h"
#include "phase_class.hpp"

#define WAV_FILE_PATH   "/home/kirill/Study/NIR/4.wav"
#define DELAY           -5

int main()
{
    uint16_t sample_rate = 44100;
    uint16_t avrg_window_num = 1;
    uint16_t size = 1 << 10;
    double tde = 0;

    auto first_array = new double*[avrg_window_num];
    auto second_array = new double*[avrg_window_num];
    for (uint16_t i = 0; i < avrg_window_num; ++i)
    {
        first_array[i] = new double[size];
        second_array[i] = new double[size];
    }

    AudioFile<double> file;
    file.load(WAV_FILE_PATH);
    uint16_t RATE = file.getSampleRate();
    uint8_t num_channels = file.getNumChannels();

    for (uint16_t window_num = 3; window_num < avrg_window_num + 3; ++window_num)
    {
        for (uint16_t sample_num = 0; sample_num < size; ++sample_num)
        {
            first_array[window_num-3][sample_num]       = file.samples[0][window_num*size + sample_num + DELAY];
            second_array[window_num-3][sample_num]      = file.samples[0][window_num*size + sample_num];
        }
    }


    phase phase_tde(avrg_window_num, size, sample_rate);
    phase_tde.set_arrays(first_array, second_array);
    phase_tde.execute();
    phase_tde.calculate_tde();
    tde = phase_tde.get_tde();
    std::cout << "PHASE_TDE:    " << tde << std::endl;


    for (uint16_t i = 0; i < avrg_window_num; ++i)
    {
        delete[] first_array[i];
        delete[] second_array[i];
    }
    delete[] first_array;
    delete[] second_array;
}
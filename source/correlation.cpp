#include "fftw3.h"
#include "cstdint"
#include "cmath"
#include "cstdio"
#include "cassert"
#include "cfloat"

#include "AudioFile.h"
#include "lib.hpp"
#include "main.hpp"

#define SAMPLE_DELAY    -1

int main()
{
    uint8_t status = 0;
    uint16_t RATE = 44100;		//sampling frequancy(Hz)
    uint16_t size = 2048;       //size of window/arrays (samples)
    uint16_t WINDOW_AVRG_NUM = 1;
    double window_size = static_cast<double>(size)/static_cast<double>(RATE);		//size of one window(sec)

    double delta_t = 1/static_cast<double>(RATE);       //delta_t in sec
    double delay = 0;                                   //result of delay's calculations

    // [WINDOW_AWRG_NUM][size]
    double          **first_array_real,
                    **second_array_real,
                    **result_array_real;            // used for correlation TDE

    first_array_real = new double*[WINDOW_AVRG_NUM];
    second_array_real = new double*[WINDOW_AVRG_NUM];
    result_array_real = new double*[WINDOW_AVRG_NUM];
    for (int i = 0; i < WINDOW_AVRG_NUM; ++i)
    {
        first_array_real[i] = new double[size];
        second_array_real[i] = new double[size];
        result_array_real[i] = new double[size];
    }


/*============================= SIGNAL GENERATION =============================*/

#if (SIGNAL == WAV_SIGNAL)
    /*=================OPEN_WAV_FILE===================*/
    ///*
    AudioFile<double> file;
    file.load(WAV_FILE_PATH);
    RATE = file.getSampleRate();
    uint8_t num_channels = file.getNumChannels();
    //uint64_t num_samples = file.getNumSamplesPerChannel();
    uint32_t num_samples = size*20;


    printf("RATE:\t\t%d\nChannels:\t%d\nSamples:\t%d\n\n", RATE, num_channels, num_samples);
    file.printSummary();
    
    
    //for (uint16_t window_num = 0; window_num < num_samples/size; ++window_num)
    for (uint16_t window_num = 2; window_num < 2 + WINDOW_AVRG_NUM; ++window_num)
    {
        for (uint16_t sample_num = 0; sample_num < size; ++sample_num)
        {
            first_array_real[window_num-2][sample_num]     = file.samples[0][window_num*size + sample_num + SAMPLE_DELAY];
            second_array_real[window_num-2][sample_num]    = file.samples[0][window_num*size + sample_num];
        }
    }

#elif (SIGNAL == IMP_SIGNAL)
    /*=======INITIALISE IMPULSE ARRAYS============*/
    for (uint16_t i = 0; i < size; i++)
    {

        if (i >= 0 && i < 4)
        {
            first_array_real[i] = 2;
            second_array_real[i] = 2;
        }
        else
        {
            first_array_real[i] = 0;
            second_array_real[i] = 0;
        }
    }

#elif (SIGNAL == SIN_SIGNAL)
    /*=======INITIALISE COSINUS ARRAYS============*/
    for (uint16_t i = 0; i < size; i++)
    {
        first_array_real[i] = 	sin(1*2*M_PI*i/size - 0.1)
        						+sin(2*2*M_PI*i/size + M_PI + 0.3)
								//+cos(3*2*M_PI*i/size + 5*M_PI/6)
								;
        second_array_real[i] = 	first_array_real[i];
    }

#else
    printf("Error choosing signal.\nAborting.\n");
    return 0;
#endif // SIGNAL

    //shift_array(size, first_array_real, SAMPLE_DELAY);
    print_real_arr(size, first_array_real[0]);
    print_real_arr(size, second_array_real[0]);

/*============================ TDE ESTIMATION ============================*/

    for (uint16_t window_num = 0; window_num < WINDOW_AVRG_NUM; ++window_num)
    {
        status = correlation(size, first_array_real[0], second_array_real[0], result_array_real[0]);
        assert(status == SUCCESS);
        shift_array(size, result_array_real[0], size/2);
        //print_real_arr(size, result_array_real[0]);
    }
    for (uint16_t sample_num = 0; sample_num < size; ++sample_num)
    {
        for (uint16_t window_num = 0; window_num < WINDOW_AVRG_NUM; ++window_num)
        {
            result_array_real[0][sample_num] += result_array_real[window_num][sample_num];
        }
    }
    normalize_real_array(size, WINDOW_AVRG_NUM, result_array_real[0]);
    print_real_arr(size, result_array_real[0]);
    status = correl_delay_value(size, result_array_real[0], delta_t, &delay);
    assert(status == 0);
    printf("Time delay:\t%.15f\n", delay);


/*================================ EXITING ===============================*/


    for (int i = 0; i < WINDOW_AVRG_NUM; ++i)
    {
        delete first_array_real[i];
        delete second_array_real[i];
        delete result_array_real[i];
    }
    delete first_array_real;
    delete second_array_real;
    delete result_array_real;

    printf("Finish\n");
    return 0;
}

#include "fftw3.h"
#include "cstdint"
#include "stdlib.h"
#include "cmath"
#include "cstdio"
#include "cassert"
#include "cfloat"
#include <vector>
#include <complex>
#include "AudioFile.h"
#include "lib.hpp"
#include "main.hpp"

#define SIGNAL      WAV_SIGNAL
#define TDE_MODE    PHASE_TDE

#define WAV_FILE_PATH   "/home/kirill/Study/NIR/4.wav"
#define SAMLE_DELAY     3

int main()
{
    uint8_t status = 0;
    uint16_t RATE = 44100;		//sampling frequancy(Hz)
    uint16_t size = 2048;       //size of window/arrays (samples)
    double window_size = static_cast<double>(size)/static_cast<double>(RATE);		//size of one window(sec)

    double delta_t = 1/static_cast<double>(RATE);                   	//delta_t in sec
    double delay = 0;                               	//result of delay's calculations


    /*
    fftw_complex    input_array[size],      //signal array nad result array
                    output_array[size],
                    check[size];
    //*/

    /*
    fftw_complex    first_array_complex[size],
                    second_array_complex[size],
                    result_array_complex[size];
    */

    double          first_array_real[size],
                    second_array_real[size],
                    result_array_real[size];



# if (SIGNAL == WAV_SIGNAL)
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
    for (uint16_t window_num = 2; window_num < 3; ++window_num)
    {
        for (uint16_t sample_num = 0; sample_num < size; ++sample_num)
        {
            ///*
            first_array_real[sample_num]     = file.samples[0][window_num*size + sample_num];
            second_array_real[sample_num]    = first_array_real[sample_num];
            //*
            /*
            first_array_real[j]     = TEST_ARRAY_1[i*size + j]
            second_array_real[j]    = TEST_ARRAY_2[i*size + j];
            //*/
        }

        /*==================CORRELATION===================*/
        /*
        status = correlation(size, first_array_real, second_array_real, result_array_real);
        assert(status == SUCCESS);
        shift_array(size, result_array_real, size/2);
        print_msg("Shifted correlation function\n");
        print_real_arr(size, result_array_real);
        status = correl_delay_value(size, result_array_real, delta_t, &delay);
        assert(status == 0);
        printf("Time delay:\t%.10f\n", delay);
        //*/

        /*==============PHASE ANALISIS================*/
        /*
        double phase_diff[size/2+1], time_diff[size/2+1];
        status = phase_delay_r2c(size, first_array_real, second_array_real, phase_diff);
        assert(status == 0);

        /*
        uint16_t harmonic_counter = 0;
        int16_t sign = 0;

        for(uint16_t i = 1; i < size/2+1; i++)
        {
            double frequancy = i/window_size;
            if (frequancy > 300 && frequancy < 3400)
            {
                time_diff[i] = phase_diff[i]*window_size/2/M_PI/i;
                if (time_diff[i] > 0)
                {
                    sign++;
                }
                if (time_diff[i] < 0)
                {
                    sign--;
                }
                //delay += time_diff[i];
                //harmonic_counter++;
            }
            else
            {
                phase_diff[i] = 0;
            }
        }
        if (sign > 0)
        {
            for (uint16_t i = 0; i < size/2+1; i++)
            {
                if (time_diff[i] > 0)
                {
                    delay += time_diff[i];
                    harmonic_counter++;
                }
            }
        }
        else
        {
            for (uint16_t i = 0; i < size/2+1; i++)
            {
                if (time_diff[i] < 0)
                {
                    delay += time_diff[i];
                    harmonic_counter++;
                }
            }
        }
        //*/
        

        //print_msg("PHASE DIFF ARRAY:\n");
        //print_real_arr(size/2+1, phase_diff);
        //print_msg("TIME ARRAY:\n");
        //print_real_arr(size/2+1, time_diff);
        //delay /= (double)(harmonic_counter);
        //printf("Time avg delay:\t%.15f\n", delay);
        //delay = 0;
        //harmonic_counter = 0;
        //*/


    }
    //*/
# elif (SIGNAL == IMP_SIGNAL)


    ///*=======INITIALISE IMPULSE ARRAYS============*/
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

#elif (SIGNAL == COS_SIGNAL)
    /*=======INITIALISE COSINUS ARRAYS============*/
    for (uint16_t i = 0; i < size; i++)
    {
        first_array_real[i] = 	cos(1*2*M_PI*i/size)
        						//+cos(2*2*M_PI*i/size + 3*M_PI/4)
								//+cos(3*2*M_PI*i/size + 5*M_PI/6)
								;
        second_array_real[i] = 	cos(1*2*M_PI*i/size)
        						//+cos(2*2*M_PI*i/size + 3*M_PI/4)
								//+cos(3*2*M_PI*i/size + 5*M_PI/6)
								;
    }

#endif // SIGNAL

    shift_array(size, second_array_real, SAMLE_DELAY);
    //print_real_arr(size, first_array_real);
    //print_real_arr(size, second_array_real);

#if (TDE_MODE == CORRELATION_TDE)

	/*==============CORRELATION================*/
    status = correlation(size, first_array_real, second_array_real, result_array_real);
    assert(status == SUCCESS);
    shift_array(size, result_array_real, size/2);
    print_real_arr(size, result_array_real);
    status = correl_delay_value(size, result_array_real, delta_t, &delay);
    assert(status == 0);
    printf("Time delay:\t%f\n", delay);

#elif (TDE_MODE == PHASE_TDE)

	/*==============PHASE ANALISIS================*/
    double phase_diff[size/2+1], time_diff[size/2+1];
    unsigned int harmonicas_counter = 0;
    status = phase_delay_r2c(size, first_array_real, second_array_real, phase_diff);
    assert(status == 0);
    for(uint16_t i = 0; i < size/2+1; i++)
    {
        double frequancy = 0;
        time_diff[i] = phase_diff[i]*window_size/2/M_PI/i;
        if (i > 0)
        {
            frequancy = i/window_size;
            if (frequancy > 300 && frequancy < 700)
            //if (frequancy > 0)
            {
                delay += time_diff[i];
                ++harmonicas_counter;
            }
            else
            {
                time_diff[i] = 0;
            }
        }
    }
    print_msg("TIME ARRAY:\n");
    print_real_arr(size/2+1, time_diff);
    delay /= harmonicas_counter;
    printf("Time avg delay:\n%.15f\n", delay);
    delay = 0;
    harmonicas_counter = 0;
    //get_pos_spectre(size, first_array);
    //print_complex_arr(size, first_array_complex);


#endif // TDE_MODE
    printf("Finish");

    return 0;
}

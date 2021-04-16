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


int main()
{
    uint8_t status = 0;
    uint16_t RATE = 44100;		//sampling frequancy(Hz)
    uint16_t size = 2048;       //size of window/arrays (samples)
    double window_size = static_cast<double>(size)/static_cast <double> (RATE);		//size of one window(sec)

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
                    result_array_real[size],
		            TEST_ARRAY_1[UINT16_MAX],
                    TEST_ARRAY_2[UINT16_MAX];

    /*
    for (uint16_t i = 0; i < UINT16_MAX; i++)
    {
        TEST_ARRAY_1[i] = double(rand()%2000 - 1000)/1000;
        TEST_ARRAY_2[i] = TEST_ARRAY_1[i];
    }
    shift_array(UINT16_MAX, TEST_ARRAY_2, 400);
    */



    /*=================OPEN_WAV_FILE===================*/
    ///*
    AudioFile<double> file;
    file.load("/home/kirill/Study/NIR/4.wav");
    RATE = file.getSampleRate();
    uint8_t num_channels = file.getNumChannels();
    //uint64_t num_samples = file.getNumSamplesPerChannel();
    uint32_t num_samples = size*20;


    printf("RATE:\t\t%d\nChannels:\t%d\nSamples:\t%d\n\n", RATE, num_channels, num_samples);
    file.printSummary();
    
    
    for (uint16_t i = 0; i < 0 + num_samples/size; i++)
    {
        for (uint16_t j = 0; j < size; j++)
        {
            ///*
            first_array_real[j]     = file.samples[0][i*size + j];
            second_array_real[j]    = file.samples[0][i*size + j + 2];
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
        ///*
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
        delay = 0;
        //harmonic_counter = 0;
        //*/


    }
    //*/



    ///*=======INITIALISE IMPULSE ARRAYS============*/
    /*
    for (uint16_t i = 0; i < size; i++)
    {

        if (i >= 0 && i < 4)
        {
            first_array_real[i] = 2;
            //second_array_real[i] = 2;
        }
        else
        {
            first_array_real[i] = 0;
            //second_array_real[i] = 0;
        }

        if (i > 2 && i < 7)
        {
            second_array_real[i] = 2;
            //first_array_real[i] = 2;
        }
        else
        {
            second_array_real[i] = 0;
            //first_array[i][REAL] = 0;
        }
    }
    //*/


    /*=======INITIALISE COSINUS ARRAYS============*/
    /*
    for (uint16_t i = 0; i < size; i++)
    {
        first_array_real[i] = 	cos(1*2*M_PI*i/size)
        						+cos(2*2*M_PI*i/size + 3*M_PI/4)
								+cos(3*2*M_PI*i/size + 5*M_PI/6)
								;
        second_array_real[i] = 	cos(1*2*M_PI*i/size)
        						+cos(2*2*M_PI*i/size + 3*M_PI/4)
								+cos(3*2*M_PI*i/size + 5*M_PI/6)
								;
    }
    shift_array(size, second_array_real, 3);
    //*/

    //print_real_arr(size, first_array_real);
    //print_real_arr(size, second_array_real);

	/*==============CORRELATION================*/
    /*
    status = correlation(size, first_array_real, second_array_real, result_array_real);
    assert(status == SUCCESS);
    shift_array(size, result_array_real, size/2);
    print_real_arr(size, result_array_real);
    status = correl_delay_value(size, result_array_real, delta_t, &delay);
    assert(status == 0);
    printf("Time delay:\t%f\n", delay);


    //*/
	/*==============PHASE ANALISIS================*/
    /*
    double phase_diff[size/2+1], time_diff[size/2+1];
    status = phase_delay_r2c(size, first_array_real, second_array_real, phase_diff);
    assert(status == 0);
    for(uint16_t i = 0; i < size/2+1; i++)
    {
        double frequancy = 0;
    	time_diff[i] = phase_diff[i]*window_size/2/M_PI/i;
        if (i > 0)
        {
            frequancy = i/window_size;
            if (frequancy > 3400 )
            {
                break;
            }
            delay += time_diff[i];
        }
    }
    print_msg("TIME ARRAY:\n");
    print_real_arr(size/2+1, time_diff);
    delay /= (double)(size/2+1);
    printf("Time avg delay:\t%.10f\n", delay);
    delay = 0;
    //*/
    //get_pos_spectre(size, first_array);
    //print_complex_arr(size, first_array_complex);
    print_msg("Finish\n");

    return 0;
}

#include "fftw3.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "assert.h"
#include "float.h"
#include <vector>
#include <complex>
#include "AudioFile.h"


#define DEBUG_ARR_REAL
//#define DEBUG_ARR_IMAG
#define DEBUG_MSG

//ERROR AND SUCCESS CODES FOR DIFFERENT FUNCTIONS:
//SUCCESS CODE:
#define SUCCESS                     (uint8_t)0x0        //universal success code for all functions

//ERROR CODES FOR FUNCTION complex_conj()
#define cc_func_ZERO_LEN            (uint8_t)0x1        //length of array equals zero
#define cc_func_NULL_ARR_PTR        (uint8_t)0x2        //pointer to input array is 0

//ERROR CODES FOR FUNCTION normalize_array()
#define na_func_ZERO_LEN            (uint8_t)0x1        //length of array equals zero
#define na_func_NULL_ARR_PTR        (uint8_t)0x2        //pointer to input array is 0

//ERROR CODES FOR FUNCTION correlation()
#define corr_func_ZERO_LEN          (uint8_t)0x1        //length of input arrays equals zero
#define corr_func_NULL_INPUT_ARR    (uint8_t)0x2        //pointer to at least one of input arrays is 0
#define corr_func_NULL_REZ_ARR      (uint8_t)0x4        //pointer to result array is 0

//ERROR CODES FOR FUNCTION swap:
#define swap_func_NULL_INPUT_PTR    (uint8_t)0x1        //pointer to at least one of input data to swap is 0
#define swap_func_ZERO_LEN          (uint8_t)0x2        //size of data equals zero

//ERROR CODES FOR FUNCTION shift_array()
#define sa_func_ZERO_LEN            (uint8_t)0x1        //length of input array equals zero
#define sa_func_NULL_INPUT_ARR      (uint8_t)0x2        //pointer to input array is 0

//ERROR CODE FOR FUNCTION correl_delay_value()
#define cdv_func_ZERO_LEN           (uint8_t)0x1        //length of input array equals zero
#define cdv_func_NULL_INPUT_ARR     (uint8_t)0x2        //pointer to input array is 0
#define cdv_func_ZERO_DELTA         (uint8_t)0x4        //sample time equals zero
#define cdv_func_NULL_DELAY_PTR     (uint8_t)0x8        //pointer to result is 0

//ERROR CODES FOR FUNCTION phase_delay()
#define pd_funk_ZERO_LEN            (uint8_t)0x1        //length of input array equals zero
#define pd_funk_NULL_INPUT_ARR      (uint8_t)0x2        //pointer to input array is 0
#define pd_funk_NULL_REZ_ARR        (uint8_t)0x4        //pointer to result arrya is 0


//MAKROS FOR REAL AND IMAGINARU PARTS OF fftw_complrx
#define REAL 0
#define IMAG 1


//DEBUG MACROS TO PRINT MESSAGES AND ARRAYS
#ifdef DEBUG_ARR_REAL

#define print_real_arr(len,array)       printf("ARRAY:\nNUM\tREAL\n");\
                                        for (uint16_t i = 0; i < len; i++)\
                                        {\
                                            printf("%.15f\n", array[i]);\
                                        }\
                                        printf("\n")

#else
#define print_real_arr(len,array) static_cast<void>(0)
#endif

#ifdef DEBUG_ARR_IMAG
#define print_complex_arr(len,array)    printf("ARRAY:\nNUM\tREAL\t\tIMAG\n");\
                                        for (uint16_t i = 0; i < len; i++)\
                                        {\
                                            printf("%d\t%.10f\t%.10f\n", i, array[i][REAL], array[i][IMAG]);\
                                        }\
                                        printf("\n")

#else
#define print_complex_arr(len,array) static_cast<void>(0)
#endif

#ifdef DEBUG_MSG
#define print_msg(msg)          printf(msg)
#else
#define print_msg(msg)      static_cast<void>(0)
#endif



//this function gets compex array and returns complex conjugate with input
//new array will be written into input
uint8_t complex_conj(   uint16_t        len,        //length of array       (in)
                        fftw_complex*   array)      //ptr to array          (in/out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)       status |= cc_func_ZERO_LEN;
    if (!array)         status |= cc_func_NULL_ARR_PTR;
    if (status)         return status;

    //FUNCTION'S BODY:
    for (uint16_t i = 0; i < len; i++)
    {
        array[i][IMAG] *= -1;
    }
    return SUCCESS;
}

//functions divides elements of array by given number
uint8_t normalize_array(    uint16_t    len,        //ammount of elements of array  (in)
                            uint16_t    coef,       //normalisation coef            (in)
                            fftw_complex* array)    //arrya                         (in/out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)       status |= na_func_ZERO_LEN;
    if (!array)         status |= na_func_NULL_ARR_PTR;
    if (status)         return status;

    //FUNCTION'S BODY:
    for (uint16_t i = 0; i < len; i++)
    {
        array[i][REAL] /= coef;
        array[i][IMAG] /= coef;
    }
    return SUCCESS;
}

uint8_t correlation(    uint16_t        len,        //size of arrays        (in)
                        double*         first,      //first array/signal    (in)
                        double*         second,     //second array/signal   (in)
                        double*         rez)        //result array          (out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)               status |= corr_func_ZERO_LEN;
    if (!first || !second)      status |= corr_func_NULL_INPUT_ARR;
    if (!rez)                   status |= corr_func_NULL_REZ_ARR;
    if (status)                 return status;

    //FUNCTION'S BODY:

    ///*
    fftw_complex* specter_1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(len/2+1));
    fftw_complex* specter_2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(len/2+1));
    //*/
    /*
    fftw_complex specter_1[len/2+1];
    fftw_complex specter_2[len/2+1];
    //*/

    //get forward fft of first array
    print_msg("Forward fft of first signal\n");
    fftw_plan plan1 = fftw_plan_dft_r2c_1d(len, first, specter_1, FFTW_ESTIMATE);
    fftw_execute(plan1);
    fftw_destroy_plan(plan1);
    print_complex_arr(len/2+1, specter_1);              //print
    normalize_array(len/2+1, len, specter_1);
    print_complex_arr(len/2+1, specter_1);              //print


    //get forward fft of second array and get complex conjugate with it
    print_msg("Forward fft of second signal\n");
    fftw_plan plan2 = fftw_plan_dft_r2c_1d(len, second, (fftw_complex*)&specter_2[0][0], FFTW_ESTIMATE);
    fftw_execute(plan2);
    fftw_destroy_plan(plan2);
    //print_complex_arr(len/2+1, specter_2);              //print
    normalize_array(len/2+1, len, specter_2);
    //print_complex_arr(len, specter_2);                  //print
    complex_conj(len/2+1, specter_2);


    //multiplication of complex arrays
    print_msg("Multiplication of arrays\n");
    for(uint16_t i = 0; i < len; i++)
    {
        specter_1[i][REAL] = specter_1[i][REAL]*specter_2[i][REAL] - specter_1[i][IMAG]*specter_2[i][IMAG];
        specter_1[i][IMAG] = specter_1[i][REAL]*specter_2[i][IMAG] + specter_1[i][IMAG]*specter_2[i][REAL];
    }
    print_complex_arr(len, specter_1);


    //backward fft of result array
    print_msg("Backward fftw\n");
    fftw_plan plan3 = fftw_plan_dft_c2r_1d(len, specter_1, rez, FFTW_ESTIMATE);
    fftw_execute(plan3);
    fftw_destroy_plan(plan3);
    //print_real_arr(len, rez);                //print

    //fftw_destroy_plan(plan1);
    //fftw_destroy_plan(plan2);
    //fftw_destroy_plan(plan3);

    //fftw_free(specter_1);
    //fftw_free(specter_2);
    return SUCCESS;
}

//functiom swaps two given elements
uint8_t swap(   void*       first,      //first data to swap        (in/out)
                void*       second,     //second data to swap       (in/out)
                uint8_t     size)       //size of data              (in)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if(!first || !second)       status |= swap_func_NULL_INPUT_PTR;
    if(size == 0)               status |= swap_func_ZERO_LEN;
    if(status)                  return status;


    //FUNCTION'S BODY:
    uint8_t* first_ = (uint8_t*)first;
    uint8_t* second_ = (uint8_t*)second;
    uint8_t tmp = 0;
    for(uint8_t i = 0; i < size; i++)
    {
        tmp = first_[i];
        first_[i] = second_[i];
        second_[i] = tmp;
    }
    return SUCCESS;
}

//function shifts right given array by given number of elements
uint8_t shift_array(    uint16_t    len,        //ammount of elements in array      (in)
                        double*     array,      //array                             (in/out)
                        uint16_t    shift)      //number of shift                   (in)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)           status |= sa_func_ZERO_LEN;
    if (!array)             status |= sa_func_NULL_INPUT_ARR;
    if (status)             return status;


    //FUCNTION'S BODY:
    for (uint16_t k = 0; k < shift; k++)
    {
        for (uint16_t i = 0; i < len-1; i++)
        {
            swap(&array[i], &array[i+1], sizeof(array[i]));
        }
    }
    return SUCCESS;
}

//this function returns value of delay between two signals in seconds
//delay is positive when the first signal outpaces the second one
//delay is negative when the first signal lags behind the second one
uint8_t correl_delay_value( uint16_t        len,        //size of array         (in)
                            double*         array,      //pointer to array      (in)
                            double          delta_t,    //delta_t in seconds    (in)
                            double*         delay)      //value of delay        (out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)       status |= cdv_func_ZERO_LEN;
    if (!array)         status |= cdv_func_NULL_INPUT_ARR;
    if (delta_t == 0)   status |= cdv_func_ZERO_DELTA;
    if (!delay)         status |= cdv_func_NULL_DELAY_PTR;
    if (status)         return status;


    //FUNCTION'S BODY
    uint16_t cor_max_i = 0;
    double cor_max = -DBL_MAX;
    for (uint16_t i = 0; i < len; i++)
    {
        if (array[i] > cor_max)
        {
            cor_max = array[i];
            cor_max_i = i;
        }
    }
    *delay = delta_t * ((len)/2 - cor_max_i);
    return SUCCESS;
}

/*
uint8_t get_pos_spectre(uint16_t len, fftw_complex* array)
{
    for (uint16_t i = 0; i < len; i++)
    {
        if (array[i][REAL] < -1e-15)
        {
            array[i][REAL] *= -1;
            array[i][IMAG] += M_PI;
        }
        while (array[i][IMAG] < -M_PI)
        {
            array[i][IMAG] += 2*M_PI;
        }
        while (array[i][IMAG] > M_PI)
        {
            array[i][IMAG] -= 2*M_PI;
        }
    }

    return SUCCESS;
}
*/

//this function returns array (via pointer) of phase harmonicas differences of two given real arrays 
uint8_t phase_delay_r2c(    uint16_t        len,                //amount of elements in given arrays    (in)
                            double*         first_array,        //first given array                     (in)
                            double*         second_array,       //second given array                    (in)
							double*			phase_diff)         //array of phase differences            (out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)                           status |= pd_funk_ZERO_LEN;
    if (!first_array || !second_array)      status |= pd_funk_NULL_INPUT_ARR;
    if (!phase_diff)                        status |= pd_funk_NULL_REZ_ARR;

    fftw_complex   specter_1[len/2 + 1];
    fftw_complex   specter_2[len/2 + 1];

    print_msg("Real data direct fft of first signal\n");
    fftw_plan plan_1 = fftw_plan_dft_r2c_1d(len, first_array, specter_1, FFTW_ESTIMATE);
    fftw_execute(plan_1);
    //fftw_destroy_plan(plan_1);
    print_complex_arr(len/2+1, specter_1);
    //normalize_array(len/2+1, len, specter_1);
    //print_complex_arr(len/2+1, specter_1);



    print_msg("Real data direct fft of second signal\n");
    fftw_plan plan_2 = fftw_plan_dft_r2c_1d(len, second_array, specter_2, FFTW_ESTIMATE);
    fftw_execute(plan_2);
    //fftw_destroy_plan(plan_2);
    print_complex_arr(len/2+1, specter_2);
    //ormalize_array(len/2+1, len, specter_2);
    //print_complex_arr(len/2+1, specter_2);

    for (uint16_t i = 0; i < len/2+1; i++)
    {
        //phase_diff[i] = acos(2*specter_1[i][REAL]) - acos(2*specter_2[i][REAL]);
    	//double a = asin(2*specter_1[i][IMAG];
    	//double b = asin(2*specter_2[i][IMAG]);
    	//phase_diff[i] = asin(2*specter_1[i][IMAG]) - asin(2*specter_2[i][IMAG]);
    	//time_diff[i] = phase_diff[i]/window_size/i;
        phase_diff[i] = atan(specter_1[i][IMAG]/specter_1[i][REAL]) - atan(specter_2[i][IMAG]/specter_2[i][REAL]);
    }
    print_msg("PHASE_DIFF\n");
    print_real_arr(len/2+1, phase_diff);
    
    //fftw_free(specter_1);
    //fftw_free(specter_2);

    //fftw_destroy_plan(plan_1);
    //fftw_destroy_plan(plan_2);

    return SUCCESS;
}


/*==============================================MAIN========================================================*/





int main(void)
{
    uint8_t status = 0;
    uint16_t RATE = 44100;								//sampling frequancy(Hz)
    uint16_t size = 1024;                          		//size of window/arrays (samples)
    double window_size = double(size)/double(RATE);		//size of one window(sec)

    double delta_t = 1/(double)RATE;                   	//delta_t in sec
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
            second_array_real[j]    = file.samples[0][i*size + j + 3];
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
        printf("\nTime delay:\t%.10f\n\n", delay);
        //*/

        /*==============PHASE ANALISIS================*/
        ///*
        double phase_diff[size/2+1] = {0}, time_diff[size/2+1] = {0};
        status = phase_delay_r2c(size, first_array_real, second_array_real, phase_diff);
        assert(status == 0);


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
        

        print_msg("PHASE DIFF ARRAY:\n");
        print_real_arr(size/2+1, phase_diff);
        print_msg("TIME ARRAY:\n");
        print_real_arr(size/2+1, time_diff);
        delay /= (double)(harmonic_counter);
        printf("Time avg delay:\t%.15f\n", delay);
        delay = 0;
        harmonic_counter = 0;
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

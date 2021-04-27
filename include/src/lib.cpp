#include "lib.hpp"
#include <cstdint>

// this function gets compex array and returns complex conjugate with input
// new array will be written into input
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

// functions divides elements of array by given number
uint8_t normalize_array(    uint16_t    len,        //ammount of elements of array  (in)
                            uint16_t    coef,       //normalisation coef            (in)
                            fftw_complex* array)    //array                         (in/out)
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

// this function returns correlation function for 2 real arrays
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

// functiom swaps two given elements
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

// function shifts right given array by given number of elements
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

// this function returns value of delay between two signals in seconds
// delay is positive when the first signal outpaces the second one
// delay is negative when the first signal lags behind the second one
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

// this function returns real phase spectrum for given complex fourier array
uint8_t get_phase_spectrum( fftw_complex*   fur,                // result of fft                (in)
                            double*         phase_spectrum,     // array for phase spectrum     (out)
                            uint16_t        N)                  // size if signal (N/2 - 1)     (in)
{
    for (uint i = 0; i < N/2 + 1; ++i)
    {
        if (fur[i][REAL] > 0)
        {
            phase_spectrum[i] = atan(fur[i][IMAG]/fur[i][REAL]);
        }
        else if (fur[i][REAL] < 0)
        {
            if (fur[i][IMAG] >= 0)
            {
                phase_spectrum[i] = atan(fur[i][IMAG]/fur[i][REAL]) + M_PI;
            }
            else
            {
                phase_spectrum[i] = atan(fur[i][IMAG]/fur[i][REAL]) - M_PI;
            }
        }
        else
        {
            if (fur[i][IMAG] > 0)
            {
                phase_spectrum[i] = M_PI_2;
            }
            else
            {
                phase_spectrum[i] = -M_PI_2;
            }
        }
    }
    return SUCCESS;
}


uint8_t unwrap_angle(double* first_angle, double* second_angle)
{
    uint8_t status = SUCCESS;
    if (first_angle == 0 || second_angle == 0)
    {
        return ua_funk_NULL_INPUT_PTR;
    }
    while (fabs(*first_angle - *second_angle) > M_PI)
    {
        if (*second_angle < *first_angle)
        {
            *second_angle += 2 * M_PI;
        }
        else
        {
            *second_angle -= 2 * M_PI;
        }
    }
    return status;
}

uint8_t unwrap_phase(double* array, uint16_t size)
{
    uint8_t status = 0;
    if (array == 0)     status |= up_func_NULL_INPUT_ARR;
    if (size == 0)      status |= up_func_NULL_ARR_LEN;
    
    for (uint16_t i = 0; i < (size / 2 + 1) - 1; ++i)
    {
        status = unwrap_angle(&array[i], &array[i+1]);
        if (status)     return up_func_UNABLE_UNWR_ANGLE;
    }
    return status;
}

uint8_t get_ampl_spectrum(  fftw_complex*   fur,
                            double*         ampl_spectrum,
                            uint16_t        N)
{
    uint8_t status = 0;
    if (0 == fur)               status |= 1;
    if (0 == ampl_spectrum)     status |= 2;
    if (0 == N)                 status |= 4;
    if (status)                 return status;

    for (uint i = 0; i < N/2 + 1; ++i)
    {
        ampl_spectrum[i] = sqrt( ( fur[i][REAL] * fur[i][REAL] ) + ( fur[i][IMAG] * fur[i][IMAG] ) );
    }
    return 0;
}

uint8_t linear_approx(  double*     x,
                        double*     y,
                        double*     a,
                        uint16_t    N)
{
    uint8_t status = SUCCESS;
    if (!x)         status |= la_func_NULL_HARMOINC_PTR;
    if (!y)         status |= la_func_NULL_PHASE_PTR;
    if (!a)         status |= la_func_NULL_COEF_PTR;
    if (0 == N)     status |= la_func_NULL_SIZE;
    if (status)     return status;
    
    double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0;
    double numerator = 0.0, denominator = 0.0;
    for (uint16_t i = 0; i < N; ++i)
    {
        sum_x += x[i];
        sum_x2 += x[i] * x[i];
        sum_y += y[i];
        sum_xy += x[i]*y[i];
    }
    numerator = (N/2 + 1) * sum_xy - sum_x * sum_y;
    denominator = (N/2 + 1) * sum_x2 - sum_x * sum_x;
    *a = numerator / denominator;
    return status;
}

// this function returns array (via pointer) of phase harmonicas differences of two given real arrays 
uint8_t phase_delay_r2c(    uint16_t        	len,                //amount of elements in given arrays    (in)
                            double*         	first_array,        //first given array                     (in)
                            double*         	second_array,       //second given array                    (in)
			                double*		        phase_diff)         //array of phase differences            (out)
{
    //DATA VALIDATION BLOCK:
    uint8_t status = SUCCESS;
    if (len == 0)                           status |= pd_funk_ZERO_LEN;
    if (!first_array || !second_array)      status |= pd_funk_NULL_INPUT_ARR;
    if (!phase_diff)                        status |= pd_funk_NULL_REZ_ARR;

    fftw_complex    specter_1[len/2 + 1];
    fftw_complex    specter_2[len/2 + 1];
    double          phase_spectrum_1[len/2 + 1];
    double          phase_spectrum_2[len/2 + 1];
    double          ampl_spectrum_1[len/2+1];
    double          ampl_spectrum_2[len/2+1];

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

    get_phase_spectrum(specter_1, phase_spectrum_1, len);
    get_phase_spectrum(specter_2, phase_spectrum_2, len);
    get_ampl_spectrum(specter_1, ampl_spectrum_1, len);
    get_ampl_spectrum(specter_2, ampl_spectrum_2, len);
    for (uint16_t i = 0; i < len/2+1; i++)
    {
        phase_diff[i] = phase_spectrum_1[i] - phase_spectrum_2[i];
    }
    unwrap_phase(phase_diff, len);
    print_msg("PHASE_DIFF\n");
    //print_real_arr(len/2+1, phase_diff);
    for (int i = 0; i < len/2+1; ++i)
        printf("%.15f\t%.15f\t%.15f\n", phase_diff[i], phase_spectrum_1[i], phase_spectrum_2[i]);
    
    //fftw_free(specter_1);
    //fftw_free(specter_2);

    //fftw_destroy_plan(plan_1);
    //fftw_destroy_plan(plan_2);

    return SUCCESS;
}
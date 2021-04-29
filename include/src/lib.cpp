#include "lib.hpp"
#include <cstdint>

uint8_t complex_conj(uint16_t len, fftw_complex* array)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)       errors |= cc_func_ZERO_LEN;
    if (!array)         errors |= cc_func_NULL_ARR_PTR;
    if (errors)         return errors;

    //FUNCTION'S BODY:
    for (uint16_t i = 0; i < len; i++)
    {
        array[i][IMAG] *= -1;
    }
    return SUCCESS;
}

uint8_t normalize_array(uint16_t len, uint16_t coef, fftw_complex* array)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)       errors |= na_func_ZERO_LEN;
    if (!array)         errors |= na_func_NULL_ARR_PTR;
    if (errors)         return errors;

    //FUNCTION'S BODY:
    for (uint16_t i = 0; i < len; i++)
    {
        array[i][REAL] /= coef;
        array[i][IMAG] /= coef;
    }
    return SUCCESS;
}

uint8_t correlation(uint16_t len, double* first, double* second, double* rez)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)               errors |= corr_func_ZERO_LEN;
    if (!first || !second)      errors |= corr_func_NULL_INPUT_ARR;
    if (!rez)                   errors |= corr_func_NULL_REZ_ARR;
    if (errors)                 return errors;

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

template <typename T> uint8_t swap(T* first, T* second)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if(!first || !second)       errors |= swap_func_NULL_INPUT_PTR;
    if(errors)                  return errors;


    //FUNCTION'S BODY:
    T temp(*first) ;
    *first = *second ;
    *second = temp ;
    return SUCCESS;
}

uint8_t shift_array(uint16_t len, double* array, uint16_t shift)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)           errors |= sa_func_ZERO_LEN;
    if (!array)             errors |= sa_func_NULL_INPUT_ARR;
    if (errors)             return errors;


    //FUCNTION'S BODY:
    for (uint16_t k = 0; k < shift; k++)
    {
        for (uint16_t i = 0; i < len-1; i++)
        {
            swap<double>(&array[i], &array[i+1]);
        }
    }
    return SUCCESS;
}

uint8_t correl_delay_value(uint16_t len, double* array, double delta_t, double* delay)      //value of delay        (out)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)       errors |= cdv_func_ZERO_LEN;
    if (!array)         errors |= cdv_func_NULL_INPUT_ARR;
    if (delta_t == 0)   errors |= cdv_func_ZERO_DELTA;
    if (!delay)         errors |= cdv_func_NULL_DELAY_PTR;
    if (errors)         return errors;


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

uint8_t get_phase_spectrum(fftw_complex* fur, double* phase_spectrum, uint16_t N)
{
    uint8_t errors = SUCCESS;
    if (fur == 0)               errors |= gps_func_NULL_FUR_ARR;
    if (phase_spectrum == 0)    errors |= gps_func_NULL_PHASE_ARR;
    if (N == 0)                 errors |= gps_func_ZERO_LEN;
    if (errors)                 return errors;

    for (uint16_t i = 0; i < N/2 + 1; ++i)
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
    uint8_t errors = SUCCESS;
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
    return errors;
}

uint8_t unwrap_phase(double* array, uint16_t size)
{
    uint8_t errors = 0;
    if (array == 0)     errors |= up_func_NULL_INPUT_ARR;
    if (size == 0)      errors |= up_func_NULL_ARR_LEN;
    
    for (uint16_t i = 0; i < (size / 2 + 1) - 1; ++i)
    {
        errors = unwrap_angle(&array[i], &array[i+1]);
        if (errors)     return up_func_UNABLE_UNWR_ANGLE;
    }
    return errors;
}

uint8_t get_ampl_spectrum(fftw_complex* fur, double* ampl_spectrum, uint16_t N)
{
    uint8_t errors = 0;
    if (0 == fur)               errors |= 1;
    if (0 == ampl_spectrum)     errors |= 2;
    if (0 == N)                 errors |= 4;
    if (errors)                 return errors;

    for (uint i = 0; i < N/2 + 1; ++i)
    {
        ampl_spectrum[i] = sqrt( ( fur[i][REAL] * fur[i][REAL] ) + ( fur[i][IMAG] * fur[i][IMAG] ) );
    }
    return 0;
}

uint8_t linear_approx(double* x, double* y, double* a, uint16_t N)
{
    uint8_t errors = SUCCESS;
    if (!x)         errors |= la_func_NULL_HARMOINC_PTR;
    if (!y)         errors |= la_func_NULL_PHASE_PTR;
    if (!a)         errors |= la_func_NULL_COEF_PTR;
    if (0 == N)     errors |= la_func_NULL_SIZE;
    if (errors)     return errors;
    
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
    return errors;
}

uint8_t phase_delay_r2c(uint16_t len, double* first_array, double* second_array, double* phase_diff)
{
    //DATA VALIDATION BLOCK:
    uint8_t errors = SUCCESS;
    if (len == 0)                           errors |= pd_funk_ZERO_LEN;
    if (!first_array || !second_array)      errors |= pd_funk_NULL_INPUT_ARR;
    if (!phase_diff)                        errors |= pd_funk_NULL_REZ_ARR;

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
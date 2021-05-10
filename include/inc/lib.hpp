///@file lib.hpp
///@brief macroses, functions and their error codes

#ifndef _NIR_LIB_
# define _NIR_LIB_

# include "fftw3.h"
# include "cstdint"
# include "cmath"
# include "cstdio"
# include "cassert"
# include "cfloat"

# define DEBUG_ARR_REAL
//# define DEBUG_ARR_IMAG
# define DEBUG_MSG

// ERROR AND SUCCESS CODES FOR DIFFERENT FUNCTIONS:
// SUCCESS CODE:
# define SUCCESS                    0		        // universal success code for all functions

// ERROR CODES FOR FUNCTION complex_conj()
# define cc_func_ZERO_LEN           ( 1 << 0 )	        // length of array equals zero
# define cc_func_NULL_ARR_PTR       ( 1 << 1 )	        // pointer to input array is 0

// ERROR CODES FOR FUNCTION normalize_array()
# define na_func_ZERO_LEN           ( 1 << 0 )        	// length of array equals zero
# define na_func_NULL_ARR_PTR       ( 1 << 1 )        	// pointer to input array is 0

// ERROR CODES FOR FUNCTION correlation()
# define corr_func_ZERO_LEN         ( 1 << 0 )        	// length of input arrays equals zero
# define corr_func_NULL_INPUT_ARR   ( 1 << 1 )        	// pointer to at least one of input arrays is 0
# define corr_func_NULL_REZ_ARR     ( 1 << 2 )        	// pointer to result array is 0

// ERROR CODES FOR FUNCTION swap()
# define swap_func_NULL_INPUT_PTR   ( 1 << 0 )	        // pointer to at least one of input data to swap is 0

// ERROR CODES FOR FUNCTION shift_array()
# define sa_func_ZERO_LEN           ( 1 << 0 )	        // length of input array equals zero
# define sa_func_NULL_INPUT_ARR     ( 1 << 1 )	        // pointer to input array is 0

// ERROR CODE FOR FUNCTION correl_delay_value()
# define cdv_func_ZERO_LEN          ( 1 << 0 )	        // length of input array equals zero
# define cdv_func_NULL_INPUT_ARR    ( 1 << 1 )	        // pointer to input array is 0
# define cdv_func_ZERO_DELTA        ( 1 << 2 )	        // sample time equals zero
# define cdv_func_NULL_DELAY_PTR    ( 1 << 3 )	        // pointer to result is 0

// ERROR CODES FOR FUNCTION get_phase_spectrum()
# define gps_func_NULL_FUR_ARR      ( 1 << 0 )          // pointer to Fourier image is 0
# define gps_func_NULL_PHASE_ARR    ( 1 << 1 )          // pointer to phase spectrum is 0
# define gps_func_ZERO_LEN          ( 1 << 2 )          // length of arrays is 0

// ERROR CODES FOR FUNCTION phase_delay()
# define pd_funk_ZERO_LEN           ( 1 << 0 )        	// length of input array equals zero
# define pd_funk_NULL_INPUT_ARR     ( 1 << 1 )	        // pointer to input array is 0
# define pd_funk_NULL_REZ_ARR       ( 1 << 2 )	        // pointer to result array is 0

// ERROR CODES FOR FUNCTION unwrap_angle()
# define ua_funk_NULL_INPUT_PTR     ( 1 << 0 )          // at least one of input pointers is 0

// ERROR CODES FOR FUNCTION unwrap_phase()
# define up_func_NULL_INPUT_ARR     ( 1 << 0 )          // pointer to input array is 0
# define up_func_NULL_ARR_LEN       ( 1 << 1 )          // length of input array is 0
# define up_func_UNABLE_UNWR_ANGLE  ( 1 << 2 )          // unable to unwrap angle

// ERROR CODES FOR FUCNTION linear_approx()
# define la_func_NULL_HARMOINC_PTR  ( 1 << 0 )          // pointer to harmonicas array is 0 (x)
# define la_func_NULL_PHASE_PTR     ( 1 << 1 )          // pointer to phase spectrum array is 0 (y)
# define la_func_NULL_COEF_PTR      ( 1 << 2 )          // pointer to slope ratio coef (TDE value) is 0 (a)
# define la_func_NULL_SIZE          ( 1 << 3 )          // size of arrays is 0


//MACROS FOR REAL AND IMAGINARY PARTS OF fftw_complex
# define REAL 0
# define IMAG 1


//DEBUG MACROS TO PRINT MESSAGES AND ARRAYS
# ifdef DEBUG_ARR_REAL
#  define print_real_arr(len,array)     printf("ARRAY:\nNUM\tREAL\n");\
                                        for (uint16_t i = 0; i < len; i++)\
                                        {\
                                            printf("%.15f\n", array[i]);\
                                        }\
                                        printf("\n")

# else
#  define print_real_arr(len,array) static_cast<void>(0)
# endif // DEBUG_ARR_REAL

# ifdef DEBUG_ARR_IMAG
#  define print_complex_arr(len,array)  printf("ARRAY:\nNUM\tREAL\t\tIMAG\n");\
                                        for (uint16_t i = 0; i < len; i++)\
                                        {\
                                            printf("%d\t%.10f\t%.10f\n", i, array[i][REAL], array[i][IMAG]);\
                                        }\
                                        printf("\n")
# else
#  define print_complex_arr(len,array) static_cast<void>(0)
# endif // DEBUG_ARR_REAL

# ifdef DEBUG_MSG
#  define print_msg(msg)        printf(msg)
# else
#  define print_msg(msg)        static_cast<void>(0)
# endif // DEBUG_MSG

///@brief This function gets compex array and returns complex conjugate with input.
/// New array will be written into input.
///@param len[in]       Length of array
///@param array[in/out] Pointer to array
///@return              Error-success code
uint8_t complex_conj(uint16_t len, fftw_complex* array);

///@brief Functions divides elements of array by given number.
///@param len[in]       Ammount of elements of array
///@param coef[in]      Normalisation coef
///@param array[in/out] Pointer to array to normalize
///@return              Error-success code
uint8_t normalize_array(uint16_t len, uint16_t coef, fftw_complex* array); 

///@brief This function returns correlation function for 2 real arrays.
///@param len[in]       Size of arrays
///@param first[in]     Pointer to the first array/signal
///@param second[in]    Pointer to the second array/signal
///@param rez[out]      Pointer to result array
///@return              Error-success code
uint8_t correlation(uint16_t len, double* first, double* second, double* rez);

///@brief Functiom swaps two given elements.
///@param first[in/out]     Pointer to the first data to swap
///@param second[in/out]    Pointer to the secnod data to swap
///@return                  Error-success code
template <typename T> uint8_t swap(T* first, T* second);

///@brief Function shifts right given array by given number of elements
///@param len[in]       Ammount of elements in array
///@param array[in/out] Pointer of array
///@param shift[in]     Number of shift (samples)
///@return              Error-success code
uint8_t shift_array(uint16_t len, double* array, uint16_t shift); 

///@brief This function returns value of delay between two signals in seconds.
/// Delay is positive when the first signal outpaces the second one.
/// Delay is negative when the first signal lags behind the second one.
///@param len[in]       Ammount of elements in array
///@param array[in]     Pointer to array
///@param delta_t[in]   Step of sampling (seconds)
///@param delay[out]    Value of delay
///@return              Error-success code
uint8_t correl_delay_value(uint16_t len, double* array, double delta_t, double* delay);

///@brief This function returns real phase spectrum for given complex fourier array.
///@param fur[in]               Fourier image of signal
///@param phase_spectrum[out]   Pointer to array for phase spectrum
///@param N[in]                 Size of source real signal
///@return                      Error-success code
uint8_t get_phase_spectrum(fftw_complex* fur, double* phase_spectrum, uint16_t N);

///@brief This function calculates amplitude specter.
///@param fur[in]               Pointer to Fourier image of signal
///@param ampl_spectrum[out]    Pointer to amplitude spectrum
///@param N[in]                 Size of source real signal
///@return                      Error-success code
uint8_t get_ampl_spectrum(fftw_complex* fur, double* ampl_spectrum, uint16_t N);

///@brief This fuction calculates unwrapped mutual phase characteristics of two given phase spectrums.
///@param len[in]           Ammount of elements in source real arrays
///@param first_array[in]   Pointer to the first phase spectrum
///@param second_array[in]  Pointer to the second phase spectrum
///@param phase_diff[out]   Pointer to unwrapped mutual phase characteristics
///@return                  Error-success code
uint8_t phase_delay_r2c(uint16_t len, double* first_array, double* second_array, double* phase_diff);

///@brief This fuction unwraps angle between two neighboring samples.
///@param first_ange[in]        Pointer to the first sample of phase spectrum
///@param second_angle[in/out]  Pointer to the second sample of phase spectrum
///@return                      Error-success code
uint8_t unwrap_angle(double* first_angle, double* secind_angle);

///@brief This function unwraps the whole phase spectrum.
///@param array[in/out] Pointer to phase spectrum
///@param size[in]      Ammount of elements in source real array
///@return              Error-success code
uint8_t unwrap_phase(double* array, uint16_t size);

///@brief This function caluclates slope ratio of mutual phase characteristics (TDE value).
///@param x[in]     Pointer to harmonicas values
///@param y[in]     Pointer to mutual phase characteristics
///@param a[out]    Pointer to slope ratio (TDE values)
///@param N[in]     Ammount of elements in source real arrays
///@return          Error-success code
uint8_t linear_approx(double* x, double* y, double* a, uint16_t N);

#endif // _NIR_LIB_
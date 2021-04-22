#ifndef _NIR_LIB_
# define _NIR_LIB_

# include "fftw3.h"
# include "cstdint"
# include "stdlib.h"
# include "cmath"
# include "cstdio"
# include "cassert"
# include "cfloat"
# include <vector>
# include <complex>

# define DEBUG_ARR_REAL
//# define DEBUG_ARR_IMAG
# define DEBUG_MSG

//ERROR AND SUCCESS CODES FOR DIFFERENT FUNCTIONS:
//SUCCESS CODE:
# define SUCCESS                    0		        //universal success code for all functions

//ERROR CODES FOR FUNCTION complex_conj()
# define cc_func_ZERO_LEN           1 << 0	        //length of array equals zero
# define cc_func_NULL_ARR_PTR       1 << 1	        //pointer to input array is 0

//ERROR CODES FOR FUNCTION normalize_array()
# define na_func_ZERO_LEN           1 << 0        	//length of array equals zero
# define na_func_NULL_ARR_PTR       1 << 1        	//pointer to input array is 0

//ERROR CODES FOR FUNCTION correlation()
# define corr_func_ZERO_LEN         1 << 0        	//length of input arrays equals zero
# define corr_func_NULL_INPUT_ARR   1 << 1        	//pointer to at least one of input arrays is 0
# define corr_func_NULL_REZ_ARR     1 << 2        	//pointer to result array is 0

//ERROR CODES FOR FUNCTION swap:
# define swap_func_NULL_INPUT_PTR   1 << 0	        //pointer to at least one of input data to swap is 0
# define swap_func_ZERO_LEN         1 << 1	        //size of data equals zero

//ERROR CODES FOR FUNCTION shift_array()
# define sa_func_ZERO_LEN           1 << 0	        //length of input array equals zero
# define sa_func_NULL_INPUT_ARR     1 << 1	        //pointer to input array is 0

//ERROR CODE FOR FUNCTION correl_delay_value()
# define cdv_func_ZERO_LEN          1 << 0	        //length of input array equals zero
# define cdv_func_NULL_INPUT_ARR    1 << 1	        //pointer to input array is 0
# define cdv_func_ZERO_DELTA        1 << 2	        //sample time equals zero
# define cdv_func_NULL_DELAY_PTR    1 << 3	        //pointer to result is 0

//ERROR CODES FOR FUNCTION phase_delay()
# define pd_funk_ZERO_LEN           1 << 0        	//length of input array equals zero
# define pd_funk_NULL_INPUT_ARR     1 << 1	        //pointer to input array is 0
# define pd_funk_NULL_REZ_ARR       1 << 2	        //pointer to result array is 0


#define UNWRAPPED                   1 << 7


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

// this function gets compex array and returns complex conjugate with input
// new array will be written into input
uint8_t complex_conj(   uint16_t        len,        //length of array       (in)
                        fftw_complex*   array);     //ptr to array          (in/out)

// functions divides elements of array by given number
uint8_t normalize_array(    uint16_t    len,        //ammount of elements of array  (in)
                            uint16_t    coef,       //normalisation coef            (in)
                            fftw_complex* array);   //array  

// this function returns correlation function for 2 real arrays
uint8_t correlation(    uint16_t        len,        //size of arrays        (in)
                        double*         first,      //first array/signal    (in)
                        double*         second,     //second array/signal   (in)
                        double*         rez);       //result array          (out)

// functiom swaps two given elements
uint8_t swap(   void*       first,      //first data to swap        (in/out)
                void*       second,     //second data to swap       (in/out)
                uint8_t     size);      //size of data              (in)

// function shifts right given array by given number of elements
uint8_t shift_array(    uint16_t    len,        //ammount of elements in array      (in)
                        double*     array,      //array                             (in/out)
                        uint16_t    shift);     //number of shift 

// this function returns value of delay between two signals in seconds
// delay is positive when the first signal outpaces the second one
// delay is negative when the first signal lags behind the second one
uint8_t correl_delay_value( uint16_t        len,        //size of array         (in)
                            double*         array,      //pointer to array      (in)
                            double          delta_t,    //delta_t in seconds    (in)
                            double*         delay);     //value of delay  

// this function returns real phase spectrum for given complex fourier array
uint8_t get_phase_spectrum( fftw_complex*   fur,                // result of fft                (in)
                            double*         phase_spectrum,     // array for phase spectrum     (out)
                            uint16_t        N);                 // size if signal (N/2 - 1)     (in)

uint8_t get_ampl_spectrum(  fftw_complex*   fur,
                            double*         ampl_spectrum,
                            uint16_t         N);

// this function returns array (via pointer) of phase harmonicas differences of two given real arrays 
uint8_t phase_delay_r2c(    uint16_t        	len,                //amount of elements in given arrays    (in)
                            double*         	first_array,        //first given array                     (in)
                            double*         	second_array,       //second given array                    (in)
			                double*		        phase_diff);        //array of phase differences            (out)

uint8_t unwrap_angle(       double*         first_angle,
                            double*         secind_angle);

uint8_t unwrap_phase(       double*         array,
                            uint16_t        size);

#endif // _NIR_LIB_
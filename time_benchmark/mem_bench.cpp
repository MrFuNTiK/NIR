#include <vector>
#include <chrono>
#include <iostream>
#include <ctime>
#include <memory>
#include <FFT/grz_forward_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <FFT/sfft_class.hpp>
#include <goerzel.h>
#include "time_benchmark.hpp"

constexpr size_t GRZ_INDEX_FIRST = 0;
constexpr size_t NUM_BENCH_ITERS = 1;

//#define RAW_GOERZEL
//#define FFT_GOERZEL_NUMBER
#define FFT_GOERZEL_PERCENT

//#define SLIDING_FFT_NUMBER
//#define SLIDING_FFT_PERCENT

//#define FFT_FFTW3



int main(int argc, char** argv)
{
    [[maybe_unused]]size_t N = 10;                  // window size, exponent of 2

    [[maybe_unused]]size_t diff = 1;  // number freqs to calculate
    [[maybe_unused]]size_t diffPercent = 1;              // percents of the half-spectrum to calculate

    [[maybe_unused]]size_t slideWidth = 1;        // number samples to update
    [[maybe_unused]]size_t slidePercent = 1;                 // overlap,  persents of window length

    if( argc < 6 )
    {
        std::cout << "Invalid usage" << std::endl;
        exit(0);
    }

    N = std::atoi( argv[ 1 ] );
    diff = std::atoi( argv[ 2 ] );
    diffPercent = std::atoi( argv[ 3 ] );
    slideWidth = std::atoi( argv[ 4 ] );
    slidePercent = std::atoi( argv[ 5 ] );

    std::srand( std::time(nullptr));

#ifdef RAW_GOERZEL
    std::cout << "Goerzel raw benchmark" << std::endl << std::endl;

    std::vector< double > data( 1 << N );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    GoerzelTF* transform = GoerzelTF_create( 1 << N );

    GoerzelTF_precalc( transform, GRZ_INDEX_FIRST, diff );
    for( size_t freq = GRZ_INDEX_FIRST; freq < GRZ_INDEX_FIRST + diff; ++freq )
    {
        GoerzelTF_set_freq_idx( transform, freq );
        for( auto& sample : data )
        {
            GoerzelTF_update( transform, sample );
        }
        GoerzelTF_result( transform );
    }
    GoerzelTF_destroy( transform );
#endif // RAW_GOERZEL

#ifdef FFT_GOERZEL_NUMBER
    std::cout << "Goerzel freq number benchmark" << std::endl << std::endl;
    std::vector< double > data( 1 << N );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform::cpu::forward::Goerzel transform( 1 << N, GRZ_INDEX_FIRST, GRZ_INDEX_FIRST + diff );
    transform.SetReal( data );
    transform.Execute();

#endif // FFT_GOERZEL_NUMBER

#ifdef FFT_GOERZEL_PERCENT
    std::vector< double > data( 0 );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }

    size_t diffSize = ( ( 1ul << N ) / 2 + 1 ) * ( diffPercent ) / 100;

    transform::cpu::forward::Goerzel transform( 1 << N, GRZ_INDEX_FIRST, GRZ_INDEX_FIRST + diffSize );
    transform.SetReal( data );
    //transform.SetReal( data );
    //transform.Execute( data );
#endif // FFT_GOERZEL_PERCENT

#ifdef SLIDING_FFT_NUMBER
    std::cout << "Sliding Fourier number benchmark" << std::endl << std::endl;

    std::vector< double > data( 1 << N );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform::cpu::forward::SFFT transform( 1 << N );
    //size_t updateSize = ( 1 << N_ ) * ( 100 - slide ) / 100 ;
    std::vector< double > updateVec( slideWidth );
    for( auto& sample : updateVec )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform.SetReal( data );
    transform.Execute();
    transform.Execute( updateVec );

#endif // SLIDING_FFT_NUMBER

#ifdef SLIDING_FFT_PERCENT
    std::cout << "Sliding Fourier percent benchmark" << std::endl << std::endl;
    std::vector< double > data( 0 );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform::cpu::forward::SFFT transform( 1 << N );
    size_t updateSize = ( 1 << N ) * ( 100 - slidePercent ) / 100 ;
    std::vector< double > updateVec( updateSize );
    for( auto& sample : updateVec )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform.SetReal( data );
    //transform.Execute();
    //transform.Execute( updateVec );
#endif // SLIDING_FFT_PERCENT

#ifdef FFT_FFTW3
    std::cout << "Fourier benchmark" << std::endl << std::endl;
    std::vector< double > data( 1 << N );
    for( auto& sample : data )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    transform::cpu::forward::FFT transform( 1 << N );
    transform.SetReal( data );
    transform.Execute();
#endif // FFT_FFTW3
}

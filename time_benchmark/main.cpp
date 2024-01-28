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
constexpr size_t NUM_BENCH_ITERS = 100;

//#define RAW_GOERZEL
//#define FFT_GOERZEL_NUMBER
#define FFT_GOERZEL_PERCENT

//#define SLIDING_FFT_NUMBER
#define SLIDING_FFT_PERCENT

//#define FFT_FFTW3

int main()
{
    [[maybe_unused]]size_t N[] = { 12, 13, 14, 15, 16, 17, 18 };                  // window size, exponent of 2

    [[maybe_unused]]size_t diff[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40, 50 };  // number freqs to calculate
    [[maybe_unused]]size_t diffPercent[] = { 100 };              // percents of the half-spectrum to calculate

    [[maybe_unused]]size_t slideWidth[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30 };        // number samples to update
    [[maybe_unused]]size_t slidePercent[] = { 0 };                 // overlap,  persents of window length

    TimeBecnmark bench;

    std::srand( std::time(nullptr));

#ifdef RAW_GOERZEL
    std::cout << "Goerzel raw benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        GoerzelTF* transform = GoerzelTF_create( 1 << N_ );
        for( auto diff_ : diff )
        {
            GoerzelTF_precalc( transform, GRZ_INDEX_FIRST, diff_ );
            for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
            {
                auto begin = std::chrono::steady_clock::now();
                for( size_t freq = GRZ_INDEX_FIRST; freq < GRZ_INDEX_FIRST + diff_; ++freq )
                {
                    GoerzelTF_set_freq_idx( transform, freq );
                    for( auto& sample : data )
                    {
                        GoerzelTF_update( transform, sample );
                    }
                    GoerzelTF_result( transform );
                }
                auto finish = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
                bench.UpdateRes( duration );
            }
            bench.CalcStats();
            std::cout << "N: " << N_ << "\t" << "diff: " << diff_ << "  \t\t"
                      << "min: " << bench.GetMin() << " \t" << "max: " << bench.GetMax() << " \t"
                      << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
            bench.Reset();
        }
        GoerzelTF_destroy( transform );
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif // RAW_GOERZEL

#ifdef FFT_GOERZEL_NUMBER
    std::cout << "Goerzel freq number benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        for( auto diff_ : diff )
        {
            transform::cpu::forward::Goerzel transform( 1 << N_, GRZ_INDEX_FIRST, GRZ_INDEX_FIRST + diff_ );
            transform.SetReal( data );
            for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
            {
                auto begin = std::chrono::steady_clock::now();
                transform.Execute();
                auto finish = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
                bench.UpdateRes( duration );
            }
            bench.CalcStats();
            std::cout << "N: " << N_ << "\t" << "diff: " << diff_ << "  \t\t"
                      << "min: " << bench.GetMin() << " \t" << "max: " << bench.GetMax() << " \t"
                      << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
            bench.Reset();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif // FFT_GOERZEL_NUMBER

#ifdef FFT_GOERZEL_PERCENT
std::cout << "Goerzel freq percent benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        for( auto diff_ : diffPercent )
        {
            size_t diffSize = ( ( 1ul << N_ ) / 2 + 1 ) * ( diff_ ) / 100;

            if( diffSize < 1 )
            {
                continue;
            }

            transform::cpu::forward::Goerzel transform( 1 << N_, GRZ_INDEX_FIRST, GRZ_INDEX_FIRST + diffSize );
            //transform.SetReal( data );
            for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
            {
                auto begin = std::chrono::steady_clock::now();
                transform.Execute( data );
                auto finish = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
                bench.UpdateRes( duration );
            }
            bench.CalcStats();
            std::cout << "N: " << N_ << "\t" << "diff: " << diffSize << "  \t\t"
                      << "min: " << bench.GetMin() << " \t" << "max: " << bench.GetMax() << " \t"
                      << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
            bench.Reset();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif // FFT_GOERZEL_PERCENT

#ifdef SLIDING_FFT_NUMBER
    std::cout << "Sliding Fourier number benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        transform::cpu::forward::SFFT transform( 1 << N_ );
        transform.SetReal( data );
        transform.Execute();
        for( const auto& slide : slideWidth )
        {
            std::vector< double > updateVec( slide );
            for( auto& sample : updateVec )
            {
                sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
            }
            transform.SetReal( data );
            transform.Execute();
            for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
            {
                auto begin = std::chrono::steady_clock::now();
                transform.Execute( updateVec );
                auto finish = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
                bench.UpdateRes( duration );
            }
            bench.CalcStats();
            std::cout << "N: " << N_ << "\t" << "slide samples: " << slide << "\t\t"
                      << "min: " << bench.GetMin() << "\t" << "max: " << bench.GetMax() << "\t"
                      << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
            bench.Reset();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif // SLIDING_FFT_NUMBER

#ifdef SLIDING_FFT_PERCENT
    std::cout << "Sliding Fourier percent benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        transform::cpu::forward::SFFT transform( 1 << N_ );
        for( const auto& slide : slidePercent )
        {
            size_t updateSize = ( 1 << N_ ) * ( 100 - slide ) / 100 ;
            std::vector< double > updateVec( updateSize );
            for( auto& sample : updateVec )
            {
                sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
            }
            //transform.SetReal( data );
            transform.Execute( data );
            for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
            {
                auto begin = std::chrono::steady_clock::now();
                transform.Execute( updateVec );
                auto finish = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
                bench.UpdateRes( duration );
            }
            bench.CalcStats();
            std::cout << "N: " << N_ << "\t" << "slide samples: " << updateSize << "\t\t"
                      << "min: " << bench.GetMin() << "\t" << "max: " << bench.GetMax() << "\t"
                      << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
            bench.Reset();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif // SLIDING_FFT_PERCENT

#ifdef FFT_FFTW3
    std::cout << "Fourier benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        transform::cpu::forward::FFT transform( 1 << N_ );
        transform.SetReal( data );
        for( size_t i = 0; i < NUM_BENCH_ITERS; ++i )
        {
            auto begin = std::chrono::steady_clock::now();
            transform.Execute();
            auto finish = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast< std::chrono::microseconds >( finish - begin).count();
            bench.UpdateRes( duration );
        }
        bench.CalcStats();
        std::cout << "N: " << N_ << "\t\t"
                  << "min: " << bench.GetMin() << "\t" << "max: " << bench.GetMax() << "\t"
                  << "mid: " << bench.GetMiddle() << "\t" << "CKO: " << bench.GetCKO() << std::endl;
        bench.Reset();
    }
    std::cout << std::endl;
#endif // FFT_FFTW3
}

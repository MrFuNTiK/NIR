#include <vector>
#include <chrono>
#include <iostream>
#include <ctime>
#include <memory>
#include <FFT/grz_forward_class.hpp>
#include <FFT/fft_forward_class.hpp>
#include <goerzel.h>
#include "time_benchmark.hpp"

constexpr size_t GRZ_INDEX_FIRST = 5;
constexpr size_t NUM_BENCH_ITERS = 100;

#define RAW_GOERZEL
#define FFT_GOERZEL
// #define FFT_FFTW3

int main()
{
    size_t N[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
    size_t diff[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40, 50 };

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

#ifdef FFT_GOERZEL
    std::cout << "Goerzel benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        for( auto diff_ : diff )
        {
            transform::cpu::grz::Forward transform( 1 << N_, GRZ_INDEX_FIRST, GRZ_INDEX_FIRST + diff_ );
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
#endif // FFT_GOERZEL

#ifdef FFT_FFTW3
    std::cout << "Fourier benchmark" << std::endl << std::endl;
    for( auto N_ : N )
    {
        std::vector< double > data( 1 << N_ );
        for( auto& sample : data )
        {
            sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
        }
        transform::cpu::fft::Forward transform( 1 << N_ );
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
#endif // FFT_FFTW3
}

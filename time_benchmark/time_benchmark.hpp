#pragma once

#include <cstdlib>
#include <limits>
#include <cmath>
//#include <util_helper/program_environment.hpp>

typedef void(bench_fn)( size_t k, size_t n, size_t diff );

class TimeBecnmark final
{
public:
    TimeBecnmark() = default;

    void UpdateRes( double time )
    {
        if( time > max )
        {
            max = time;
        }
        if( time < min )
        {
            min = time;
        }
        sum_time += time;
        sum2_time += time * time;
        ++num_iters;
    }

    void CalcStats()
    {
        Mx = sum_time / num_iters;
        Mx2 = sum2_time / num_iters;
        M2x = pow( Mx, 2 );
        Dx = Mx2 - M2x;
        CKO = sqrt( Dx );
    }

    void Reset()
    {
        num_iters = 0;
        sum_time = 0.;
        sum2_time = 0.;
        min = std::numeric_limits<double>::max();
        max = std::numeric_limits<double>::min();

        Mx = 0.;
        M2x = 0.;
        Mx2 = 0.;
        Dx = 0.;
        CKO = 0.;
    }

    double GetMin() const
    {
        return min;
    }

    double GetMax() const
    {
        return max;
    }

    double GetMiddle() const
    {
        return Mx;
    }

    double GetCKO() const
    {
        return CKO;
    }

private:
    size_t num_iters = 0;
    double sum_time = 0.;
    double sum2_time = 0.;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    double Mx = 0.;
    double M2x = 0.;
    double Mx2 = 0.;
    double Dx = 0.;
    double CKO = 0.;
};

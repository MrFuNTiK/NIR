#include <gtest/gtest.h>
#include <tuple>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <TDE/TDE_class.hpp>
#include <util_helper/program_environment.hpp>

using namespace tde;
using namespace utility_helpers::program_environment;
using PE = ProgramEnvironment;

static const uint16_t DEFAULT_SAMPLE_RATE = 44100;
static const uint16_t DEFAULT_WINDOW_SIZE = 1 << 10;
static const uint16_t DEFAULT_AVRG_NUM = 5;

class calc_test : public testing::TestWithParam<std::tuple<WEIGHTING_FN_TYPE, TDE_METH>>
{
public:
    calc_test() = default;
    ~calc_test() = default;

    void SetUp();
    void TearDown();

    std::vector<double> vec1, vec2;
    std::unique_ptr<iTDE> calc;
};

void calc_test::SetUp()
{
    std::srand( std::time(nullptr));
    vec1.resize(DEFAULT_WINDOW_SIZE);
    vec2.resize(DEFAULT_WINDOW_SIZE);
    for( auto& sample : vec1 )
    {
        sample = static_cast<double>( std::rand() ) / RAND_MAX - 0.5;
    }
    std::memcpy( &vec2[ 0 ], &vec1[ 0 ], sizeof( double ) * vec1.size() );
    auto& pe = ProgramEnvironment::GetInstance();
    ASSERT_NO_THROW(pe.SetWindowSize(DEFAULT_WINDOW_SIZE));
    ASSERT_NO_THROW(pe.SetSampleRate(DEFAULT_SAMPLE_RATE));
    ASSERT_NO_THROW(pe.SetWeightingFunction(std::get<0>(GetParam())));
    ASSERT_NO_THROW(pe.SetMethodTDE(std::get<1>(GetParam())));
    ASSERT_NO_THROW(pe.SetWinAvrgNum(DEFAULT_AVRG_NUM));
}

void calc_test::TearDown()
{}

TEST_P(calc_test, leak_test)
{
    ASSERT_NO_THROW(calc.reset(PE::GetInstance().CreateCalculator()));
    ASSERT_NE(calc.get(), nullptr);
    for( int i = 0; i < 10; ++i )
    {
        for( int j = 0; j < PE::GetInstance().GetWinAvrgNum(); ++j )
        {
            calc->Update(vec1, vec2);
        }
        calc->Conclude();
        EXPECT_NEAR( calc->GetTde(), 0., 0.00001 );
    }
}

INSTANTIATE_TEST_SUITE_P(TDE_tests, calc_test,
                         ::testing::Combine(
                             ::testing::Values(tde::COHERENCE, tde::NONE),
                             ::testing::Values(GCC_TDE, GPS_TDE, GPS_GRZ_TDE)
                         ));

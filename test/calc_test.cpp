#include <gtest/gtest.h>
#include <tuple>
#include <memory>
#include <TDE/TDE_class.hpp>
#include <util_helper/program_environment.hpp>

using PE = program_environment;

static const uint16_t DEFAULT_SAMPLE_RATE = 44100;
static const uint16_t DEFAULT_WINDOW_SIZE = 1 << 10;
static const uint16_t DEFAULT_AVRG_NUM = 5;

class calc_test : public testing::TestWithParam<std::tuple<weighting_func, tde_meth>>
{
public:
    calc_test();
    ~calc_test();

    void SetUp();
    void TearDown();

    std::vector<double> vec1, vec2;
    std::unique_ptr<TDE_calc> calc;
};

calc_test::calc_test()
{}

calc_test::~calc_test()
{}

void calc_test::SetUp()
{
    vec1.resize(DEFAULT_WINDOW_SIZE);
    vec2.resize(DEFAULT_WINDOW_SIZE);
    auto pe = PE::GetInstance();
    ASSERT_NO_THROW(pe->SetWindowSize(DEFAULT_WINDOW_SIZE));
    ASSERT_NO_THROW(pe->SetSampleRate(DEFAULT_SAMPLE_RATE));
    ASSERT_NO_THROW(pe->SetWeightingFunction(std::get<0>(GetParam())));
    ASSERT_NO_THROW(pe->SetMethodTDE(std::get<1>(GetParam())));
    ASSERT_NO_THROW(pe->SetWinAvrgNum(DEFAULT_AVRG_NUM));
}

void calc_test::TearDown()
{}

TEST_P(calc_test, leak_test)
{
    ASSERT_NO_THROW(calc.reset(PE::GetInstance()->CreateCalculator()));
    ASSERT_NE(calc.get(), nullptr);
    for( int i = 0; i < 10; ++i )
    {
        for( int j = 0; j < PE::GetInstance()->GetWinAvrgNum(); ++j )
        {
            calc->update(vec1, vec2);
        }
        calc->conclude();
    }
}

INSTANTIATE_TEST_SUITE_P(TDE_tests, calc_test,
                         ::testing::Combine(
                             ::testing::Values(COHERENCE, NONE),
                             ::testing::Values(GCC_TDE, GPS_TDE)
                         ));

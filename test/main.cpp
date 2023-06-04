#include <gtest/gtest.h>
#include <logger/logger.hpp>

int main()
{
#ifdef ENABLE_LOGGER
    try
    {
        auto& logger_ = logger::Logger::GetInstance();
        logger_.SetEvents( logger::EVENTS::TDE_CALC | logger::EVENTS::ERROR | logger::EVENTS::DEBUG );
        logger_.SetTrace(  "log.txt" );
        logger_.Initialize();
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
#endif
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <logger/logger.hpp>
#include "test_env.hpp"

int main(int argc, char** argv)
{
#ifdef ENABLE_LOGGER
    try
    {
        auto& logger_ = logger::Logger::GetInstance();
        logger_.SetEvents( logger::EVENTS::DEBUG );
        logger_.SetTrace( "stdout" );
        logger_.Initialize();
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
#endif
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new TestEnvironment( argc, argv ) );
    return RUN_ALL_TESTS();
}

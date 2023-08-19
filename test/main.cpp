#include <gtest/gtest.h>
#include <logger/logger.hpp>

int main(int argc, char** argv)
{
#ifdef ENABLE_LOGGER
    try
    {
        auto& logger_ = logger::Logger::GetInstance();
        logger_.SetEvents( logger::EVENTS::DEBUG );
        logger_.SetTrace( "log.txt" );
        logger_.Initialize();
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
#endif
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

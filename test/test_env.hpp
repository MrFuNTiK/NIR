#include <util_helper/program_environment.hpp>
#include <gtest/gtest.h>

#pragma once

using namespace utility_helpers::program_environment;

class TestEnvironment : public testing::Environment
{
public:
    TestEnvironment( int argc, char* argv[] )
    {
        if( argc > 1 )
        {
            ProgramEnvironment::GetInstance().SetUpByArgs(argc, argv);
        }
    }
};

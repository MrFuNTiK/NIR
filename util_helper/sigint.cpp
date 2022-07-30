#include <csignal>
#include <iostream>
#include <util_helper/program_environment.hpp>
#include <logger/logger.hpp>

void SIGINT_handler(int)
{
    TRACE_EVENT(EVENTS::EVENT, "SIGINT recieved");
    program_environment::GetInstance()->SetExecutable(false);
}

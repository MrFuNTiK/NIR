#include <csignal>
#include <iostream>
#include <util_helper/program_environment.hpp>

void SIGINT_handler(int)
{
    std::cout << "SIGINT recieved\n";
    program_environment::GetInstance()->SetExecutable(false);
}

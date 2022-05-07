#include <csignal>
#include <iostream>
#include "program_environment.hpp"

void SIGINT_handler(int sigNum)
{
    std::cout << "SIGINT recieved\n";
    std::shared_ptr<program_environment>pe = program_environment::GetInstance();
    pe->SetExecutable(false);
}

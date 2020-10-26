#include <spi/Replay.hpp>

#include "cpplib_cpplib_service.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    cpplib::cpplib_start_service();
    int status = -1;

    try
    {
        spi::mainReplay(argc, argv);
        status = 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    cpplib::cpplib_stop_service();
    return status;
}

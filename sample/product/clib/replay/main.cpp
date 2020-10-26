#include <spi/Replay.hpp>

#include "clib_clib_service.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    clib::clib_start_service();
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
    clib::clib_stop_service();
    return status;
}

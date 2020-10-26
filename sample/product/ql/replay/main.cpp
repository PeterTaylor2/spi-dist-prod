#include <spi/Replay.hpp>

#include "ql_ql_service.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    ql::ql_start_service();
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
    ql::ql_stop_service();
    return status;
}

#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "server.hpp"
#include "logger.hpp"

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        std::cerr << "Usage: main <host> <port>\n";
        return 1;
    }

    scalable::server::Logger logger;
    logger.log("INFO", "main", "program started");

    try
    {
        boost::asio::io_context io;

        logger.log("INFO", "main", "Starting server...");
        scalable::server::server server(io, argv[1], argv[2]);
        io.run();
        logger.log("INFO", "main", "Server stopped");
    }
    catch(const std::exception& e)
    {
        logger.log("ERROR", "main", e.what());
        return 1;
    }
    
    return 0;
}
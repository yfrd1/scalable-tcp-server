#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "server.hpp"
#include "logger.hpp"

int main(int argc, char* argv[])
{
    if(argc!=4)
    {
        std::cerr << "Usage: ./server <host> <port> <thread_pool_size>\n";
        return 1;
    }

    scalable::server::logger logger_;
    logger_.log("INFO", "main", "program started");

    try
    {
        boost::asio::io_context io;

        logger_.log("INFO", "main", "Starting server...");
        scalable::server::server server(io, argv[1], argv[2] , std::stoi(argv[3]));
        server.run();

        logger_.log("INFO", "main", "Server stopped");
    }
    catch(const std::exception& e)
    {
        logger_.log("ERROR", "main", e.what());
        return 1;
    }
    
    return 0;
}
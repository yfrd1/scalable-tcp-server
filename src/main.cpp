#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "server.hpp"
#include "logger.hpp"
#include "config.hpp"

using scalable::server::server;
using scalable::server::config;
using scalable::server::logger;

int main(int argc, char* argv[])
{

    std::shared_ptr<config> cnf { 
        std::make_shared<config>("config.json") };

    if(!cnf->load())
    {
        std::cerr << "config.json file not found\n";
        return 1;
    }

    logger logger_;
    logger_.log("INFO", "main", "program started");

    try
    {
        boost::asio::io_context io;
        logger_.log("INFO", "main", "Starting server...");
        server server(io, cnf);
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
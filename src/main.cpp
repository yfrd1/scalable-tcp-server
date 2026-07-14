#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "server/server.hpp"
#include "logger/logger.hpp"
#include "config/config.hpp"

using scalable::server::server;
using scalable::server::config;
using scalable::server::logger;
using LogLevel = scalable::server::logger::LogLevel;

int main(int argc, char* argv[])
{

    config cnf("config.json");

    if(!cnf.load())
    {
        std::cerr << "config.json file not found\n";
        return 1;
    }

    std::shared_ptr<logger> logger_;
    try
    {
        logger_= std::make_shared<logger>(cnf);
        if(!logger_->getFileFolders())
        {
            throw std::invalid_argument("There is an error in path, name or extension of log file");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    logger_->log(LogLevel::Alert, "main", "program started");

    try
    {
        boost::asio::io_context io;
        logger_->log(LogLevel::Info, "main", "Starting server...");
        server server(io, cnf, logger_);
        server.run();

        logger_->log(LogLevel::Info, "main", "Server stopped");
    }
    catch(const std::exception& e)
    {
        logger_->log(LogLevel::Error, "main", e.what());
        return 1;
    }
    
    return 0;
}
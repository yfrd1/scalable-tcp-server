#include "application.hpp"
#include "logger/logger.hpp"

namespace scalable {
namespace server {

    using LogLevel = Logger::LogLevel;

    Application::Application() :
    io_context_(),
    work_guard_(boost::asio::make_work_guard(io_context_)),
    signals_(io_context_),
    config_("config.json"),
    thread_pool_(),
    buffer_pool_(),
    router_()
    {

        signals_.add(SIGHUP);
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
        #ifdef SIGQUIT
        signals_.add(SIGQUIT);
        #endif
        do_await_stop();

        if(!config_.load())
            throw std::runtime_error("config.json file not found");

        logger_=std::make_shared<Logger>(config_);

        if(!logger_->getFileFolders())
            throw std::invalid_argument("There is an error in path, name or extension of log file");

        server_=std::make_unique<Server>(
            io_context_, config_, logger_);

        
    }

    void Application::run()
    {
     
        // Start application services
        logger_->log(LogLevel::Alert, "Application", "program started");
        logger_->log(LogLevel::Info, "Application", "Running server...");

        server_->start_accept();
        server_->start_workers();

        logger_->log(LogLevel::Info, "Application", "Server stopped");
    }

    void Application::stop()
    {
        logger_->log(LogLevel::Info, "Application", "Application shutdown started");

        server_->stop();
        work_guard_.reset();
        io_context_.stop();
    }

    void Application::do_await_stop()
    {
        signals_.async_wait(
            [this](boost::system::error_code ec, int signo)
            {
                if(ec)
                {
                    return;
                }
                else
                {
                    switch(signo)
                    {
                        case SIGTERM:
                        case SIGINT:
                        #ifdef SIGQUIT
                        case SIGQUIT:
                        #endif
                            stop();
                            return;
                        case SIGHUP:
                            try
                            {
                                config_.reload();   
                            }
                            catch(const std::exception& e)
                            {
                                std::cerr << e.what() << '\n';
                            }
                            do_await_stop();
                            return;
                    }
                }
            }
        );
    }



}
}
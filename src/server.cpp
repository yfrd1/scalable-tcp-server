#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include "logger.hpp"
#include "server.hpp"
#include "connection.hpp"
#include "config.hpp"

using boost::asio::ip::tcp;
using LogLevel = scalable::server::logger::LogLevel;

namespace scalable {
namespace server {

    server::server(boost::asio::io_context& io, config& cnf, std::shared_ptr<logger> log) :
        io_context(io), 
        work_guard(boost::asio::make_work_guard(io_context)),
        acceptor(io), signals(io),
        config_(cnf),
        logger_(log)
    {
        logger_->log(LogLevel::Info, "server", "initializing server");

        signals.add(SIGINT);
        signals.add(SIGTERM);
        #ifdef SIGQUIT
        signals.add(SIGQUIT);
        #endif

        do_await_stop();

        std::string host = config_.get_string("server.address");
        int port = config_.get_int("server.port");
        bool reuse_address = config_.get_bool("server.reuse_address");
        int backlog = config_.get_int("server.backlog");
        max_connections=config_.get_int("connections.max_connections");

        tcp::endpoint server_ep(
            boost::asio::ip::address::from_string(host),
            static_cast<unsigned short>(port));

        logger_->log(LogLevel::Info, "server", "resolved endpoint " + host + ":" + std::to_string(port));

        acceptor.open(server_ep.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(reuse_address));
        acceptor.bind(server_ep);
        acceptor.listen(backlog);

        logger_->log(LogLevel::Info, "server", "listening on " + host + ":"  + std::to_string(port));

        do_accept();
    }

    void server::run()
    {
        int thread_count = 
            config_.get_int("thread_pool.thread_count");

        std::vector<std::thread> threads;
        for(size_t i=0; i<thread_count; ++i)
        {
            threads.emplace_back([this]{ io_context.run(); });
        }

        for(size_t i=0; i<thread_count; ++i)
        {
            threads[i].join();
        }
    }

    void server::do_accept()
    {
        acceptor.async_accept(
            boost::asio::make_strand(io_context),
            [this](boost::system::error_code ec, tcp::socket sock)
            {
                if(!acceptor.is_open())
                {
                    return;
                }

                if(!ec)
                {

                    if(add_connection())
                    {
                        tcp::endpoint ep = sock.remote_endpoint();
                        logger_->log(LogLevel::Info, "server", "client connected: " +
                            ep.address().to_string() + ":" + 
                                std::to_string(ep.port()));

                        std::make_shared<connection>(std::move(sock), config_, logger_)->start();
                    }
                    else
                    {
                        logger_->log(
                        LogLevel::Warning,
                        "server",
                        "Connection rejected, maximum connections reached: "+
                        std::to_string(max_connections));

                        boost::system::error_code ec;
                        sock.close(ec);
                    }
                }
                else
                {
                    logger_->log(LogLevel::Error, "server", ec.message());
                }

                do_accept();
            }
        );
    }

    void server::do_await_stop()
    {
        signals.async_wait(
            [this](boost::system::error_code /* ec */, int /* signo */)
            {
                stop();
            }
        );
    }

    void server::stop()
    {
        logger_->log(LogLevel::Info, "server", "shutdown started");
                
        acceptor.close();
        work_guard.reset();
        io_context.stop();
        
    }

    bool server::add_connection()
    {
        if(active_connections>=max_connections)
        {
            return false;
        }

        ++active_connections;
        return true;
    }

    void server::remove_connection()
    {
        if(active_connections>0)
        {
            --active_connections;
        }
    }

}
}
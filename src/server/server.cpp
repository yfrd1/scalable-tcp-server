#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include "server.hpp"
#include "logger/logger.hpp"
#include "config/config.hpp"
#include "session/session.hpp"

using boost::asio::ip::tcp;
using LogLevel = scalable::server::Logger::LogLevel;

namespace scalable {
namespace server {

    Server::Server(boost::asio::io_context& io, 
        Config& cnf, 
        std::shared_ptr<Logger> log) :
        io_context_(io),
        acceptor(io),
        config_(cnf),
        logger_(log)
    {
        logger_->log(LogLevel::Info, "server", "initializing server");

        std::string host = config_.get_string("server.address");
        int port = config_.get_int("server.port");
        bool reuse_address = config_.get_bool("server.reuse_address");
        int backlog = config_.get_int("server.backlog");
        max_sessions=config_.get_int("sessions.max_sessions");

        tcp::endpoint server_ep(
            boost::asio::ip::make_address(host),
            static_cast<unsigned short>(port));

        logger_->log(LogLevel::Info, "server", "resolved endpoint " + host + ":" + std::to_string(port));

        acceptor.open(server_ep.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(reuse_address));
        acceptor.bind(server_ep);
        acceptor.listen(backlog);

        logger_->log(LogLevel::Info, "server", "listening on " + host + ":"  + std::to_string(port));
    }


    void Server::start_workers()
    {
        int thread_count = 
            config_.get_int("thread_pool.thread_count");
        std::vector<std::thread> threads;
        for(size_t i=0; i<thread_count; ++i)
        {
            threads.emplace_back([this]{ io_context_.run(); });
        }
        for(size_t i=0; i<thread_count; ++i)
        {
            threads[i].join();
        }
    }

    void Server::start_accept()
    {
        if(acceptor.is_open())
            do_accept();
    }

    void Server::do_accept()
    {
        acceptor.async_accept(
            boost::asio::make_strand(io_context_),
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

                        std::make_shared<session>(std::move(sock), config_, logger_)->start();
                    }
                    else
                    {
                        logger_->log(
                        LogLevel::Warning,
                        "server",
                        "session rejected, maximum sessions reached: "+
                        std::to_string(max_sessions));

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


    void Server::stop()
    {
        logger_->log(LogLevel::Info, "Server", "Stop acceptor");
        acceptor.close();
    }

    bool Server::add_connection()
    {
        if(active_connections>=max_sessions)
        {
            return false;
        }

        ++active_connections;
        return true;
    }

    void Server::remove_connection()
    {
        if(active_connections>0)
        {
            --active_connections;
        }
    }

}
}
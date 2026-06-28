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

namespace scalable {
namespace server {

    server::server(boost::asio::io_context& io, std::shared_ptr<config> cnf) :
        io_context(io), acceptor(io), signals(io),
        config_(cnf)
    {
        logger_.log("INFO", "server", "initializing server");

        signals.add(SIGINT);
        signals.add(SIGTERM);
        #ifdef SIGQUIT
        signals.add(SIGQUIT);
        #endif

        do_await_stop();

        std::string host = config_->get_string("server.address");
        int port = config_->get_int("server.port");
        bool reuse_address = config_->get_bool("server.reuse_address");
        int backlog = config_->get_int("server.backlog");

        tcp::endpoint server_ep(
            boost::asio::ip::address::from_string(host),
            static_cast<unsigned short>(port));

        logger_.log("INFO", "server", "resolved endpoint " + host + ":" + std::to_string(port));

        acceptor.open(server_ep.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(reuse_address));
        acceptor.bind(server_ep);
        acceptor.listen(backlog);

        logger_.log("INFO", "server", "listening on " + host + ":"  + std::to_string(port));

        do_accept();
    }

    void server::run()
    {
        int thread_count = 
            config_->get_int("thread_pool.thread_count");

        std::vector<std::thread> threads;
        for(size_t i=0; i<thread_count; ++i)
        {
            threads.emplace_back([&]{ io_context.run(); });
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

                    tcp::endpoint ep = sock.remote_endpoint();
                    logger_.log("INFO", "server", "client connected: " +
                        ep.address().to_string() + ":" + 
                            std::to_string(ep.port()));

                    std::make_shared<connection>(std::move(sock))->start();
                }
                else
                {
                    logger_.log("ERROR", "server", ec.message());
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
                logger_.log("INFO", "server", "shutdown signal received");
                acceptor.close();
                logger_.log("INFO", "server", "acceptor closed");
            }
        );
    }

}
}
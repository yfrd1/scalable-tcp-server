#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <thread>
#include "logger.hpp"
#include "server.hpp"
#include "connection.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

    server::server(boost::asio::io_context& io, 
        const std::string& host, const std::string& port,
        size_t thread_pool_size) :
        io_context(io), acceptor(io), signals(io),
        max_thread_pool_size(thread_pool_size)
    {
        logger.log("INFO", "server", "initializing server");

        signals.add(SIGINT);
        signals.add(SIGTERM);
        #ifdef SIGQUIT
        signals.add(SIGQUIT);
        #endif

        do_await_stop();

        tcp::resolver resolve(io);
        tcp::endpoint endpoint = *resolve.resolve(host, port).begin();

        logger.log("INFO", "server", "resolved endpoint " + host + ":" + port);

        acceptor.open(endpoint.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen();

        logger.log("INFO", "server", "listening on " + host + ":" + port);

        do_accept();
    }

    void server::run()
    {
        std::vector<std::thread> threads;
        for(size_t i=0; i<max_thread_pool_size; ++i)
        {
            threads.emplace_back([&]{ io_context.run(); });
        }

        for(size_t i=0; i<max_thread_pool_size; ++i)
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
                    logger.log("INFO", "server", "client connected: " +
                        ep.address().to_string() + ":" + 
                            std::to_string(ep.port()));

                    std::make_shared<connection>(std::move(sock))->start();
                }
                else
                {
                    logger.log("ERROR", "server", ec.message());
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
                logger.log("INFO", "server", "shutdown signal received");
                acceptor.close();
                logger.log("INFO", "server", "acceptor closed");
            }
        );
    }

}
}
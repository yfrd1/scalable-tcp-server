#include <boost/asio.hpp>
#include <string>
#include "server.hpp"
#include "connection.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

    server::server(boost::asio::io_context& io, 
        const std::string& host, const std::string& port) :
        acceptor(io), signals(io)
    {
        signals.add(SIGINT);
        signals.add(SIGTERM);
        #ifdef SIGQUIT
        signals.add(SIGQUIT);
        #endif

        do_await_stop();

        tcp::resolver resolve(io);
        tcp::endpoint endpoint = *resolve.resolve(host, port).begin();

        acceptor.open(endpoint.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen();

        do_accept();
    }

    void server::do_accept()
    {
        acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket sock)
            {
                if(!acceptor.is_open())
                {
                    return;
                }

                if(!ec)
                {
                    std::make_shared<connection>(std::move(sock))->start();
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
                acceptor.close();
            }
        );
    }

}
}
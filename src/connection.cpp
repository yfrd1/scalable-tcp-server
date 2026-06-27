#include <boost/asio.hpp>
#include <memory>
#include <array>
#include "connection.hpp"
#include "logger.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

    connection::connection(tcp::socket sock) : socket(std::move(sock))
    {
        logger_.log("INFO", "connection", "new connection created");
    }

    void connection::start()
    {
        logger_.log("INFO", "connection", "connection started reading");
        read_data();
    }

    void connection::read_data()
    {
        auto self { shared_from_this() };
        socket.async_read_some(
            boost::asio::buffer(data),
            [this, self](boost::system::error_code ec, size_t bytes)
            {
                if(!ec)
                {
                    logger_.log("DEBUG", "connection", "data received: " + std::to_string(bytes));
                    write_data(bytes);
                }
                else
                {
                    logger_.log("ERROR", "connection", ec.message());
                    return;
                }
            }
        );
    }

    void connection::write_data(size_t length)
    {
        auto  self { shared_from_this() };
        boost::asio::async_write(
            socket, 
            boost::asio::buffer(data, length),
            [this, self](boost::system::error_code ec, size_t)
            {
                if(ec)
                {
                    logger_.log("ERROR", "connection", "write failed: " + ec.message());
                }
            }
        );
    }

}
}
#include <boost/asio.hpp>
#include <memory>
#include <array>
#include "connection.hpp"
#include "logger.hpp"
#include "config.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

    connection::connection(tcp::socket sock, 
        std::shared_ptr<config>cnf) : 
        socket(std::move(sock)),
        config_(cnf),
        max_message_size_bytes{config_->get_int("connections.max_message_size_bytes")},
        data(max_message_size_bytes)
    {
        logger_.log("INFO", "connection", "new connection created");
    }

    void connection::start()
    {

        boost::system::error_code ec;
        bool no_dely = config_->get_bool("socket.tcp_no_delay");
        bool keep_alive = config_->get_bool("socket.keep_alive");
        socket.set_option(tcp::no_delay(no_dely), ec);
        socket.set_option(tcp::socket::keep_alive(keep_alive), ec);
        if(ec)
        {
            logger_.log("ERROR", "connection", ec.message());
            return;
        }

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
                    if(bytes>=max_message_size_bytes)
                    {
                            logger_.log("ERROR",
                            "connection",
                            "message too large: "+std::to_string(bytes)+" bytes");
                            return;
                    }

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
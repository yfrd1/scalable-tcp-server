#include <boost/asio.hpp>
#include <memory>
#include <array>
#include "connection.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

    connection::connection(tcp::socket sock) : socket(std::move(sock))
    {

    }

    void connection::start()
    {
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
                    write_data(bytes);
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
            [this, self](boost::system::error_code, size_t)
            {
                // Do nothing
            }
        );
    }

}
}
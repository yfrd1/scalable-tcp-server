#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Writer
{
public:
    explicit Writer(tcp::socket& socket);
private:
    tcp::socket& socket_;
};

}
}
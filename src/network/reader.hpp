#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Reader
{
public:
    explicit Reader(tcp::socket& socket);
    
private:
    tcp::socket& socket_;
};

}
}
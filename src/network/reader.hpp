#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <cstdint>
#include <vector>

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Session;

class Reader : public std::enable_shared_from_this<Reader>
{
public:
    explicit Reader(tcp::socket& socket, 
        std::weak_ptr<Session> session);

    void read_length();
    void read_body();

private:
    std::weak_ptr<Session> session_;
    tcp::socket& socket_;
    uint32_t packet_length_;
    std::vector<uint8_t> packet_buffer_;

};

}
}
#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <cstdint>
#include <deque>

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Session;

class Writer : public std::enable_shared_from_this<Writer>
{
public:
    explicit Writer(tcp::socket& socket,
        std::weak_ptr<Session> session);

    void add_packet(std::vector<uint8_t> packet);
    void write();
    
private:
    tcp::socket& socket_;
    std::weak_ptr<Session> session_;
    std::deque<std::vector<uint8_t>> queue_buffer_;
};

}
}
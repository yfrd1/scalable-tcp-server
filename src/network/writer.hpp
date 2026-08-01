#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <deque>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::steady_timer;

namespace scalable {
namespace server {

class Session;
class Config;

class Writer : public std::enable_shared_from_this<Writer>
{
public:
    explicit Writer(tcp::socket& socket,
        std::weak_ptr<Session> session, Config& config);

    void add_packet(std::vector<uint8_t> packet);
    void write();
    
private:
    void check_write_deadline();
    
    tcp::socket& socket_;
    std::weak_ptr<Session> session_;
    std::deque<std::vector<uint8_t>> queue_buffer_;

    steady_timer write_timer_;
    int seconds_write = 10;
};

}
}
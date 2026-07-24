#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <cstdint>
#include <vector>
#include "config/config.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Session;

class Reader : public std::enable_shared_from_this<Reader>
{
public:
    explicit Reader(tcp::socket& socket, 
        std::weak_ptr<Session> session, Config& config);

    void read_length();
    void read_body();

private:
    void check_read_deadline();
    void check_idle_deadline();
    
    std::weak_ptr<Session> session_;
    tcp::socket& socket_;
    uint32_t packet_length_;
    std::vector<uint8_t> packet_buffer_;
    
    boost::asio::steady_timer read_timer;
    boost::asio::steady_timer idle_timer;
    int read_seconds = 0;
    int idle_seconds = 0;
    
};

}
}
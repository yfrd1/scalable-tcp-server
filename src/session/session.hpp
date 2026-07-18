#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <cstdint>
#include "logger/logger.hpp"
#include "config/config.hpp"
#include "network/reader.hpp"
#include "network/writer.hpp"
#include "common/packet.hpp"


using boost::asio::ip::tcp;
using boost::asio::steady_timer;
using scalable::common::Packet;

namespace scalable {
namespace server {

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket, Config& config, 
        std::shared_ptr<Logger> logger);
    void start();

    void read_packet();
    void write_packet(std::vector<uint8_t> packet);
    void stop();
    void on_packet(std::vector<uint8_t> packet);

private:
    tcp::socket socket_;
    std::shared_ptr<Logger> logger_;
    Config& config_;
    
    std::shared_ptr<Reader> reader_;
    std::shared_ptr<Writer> writer_;

};

}
}

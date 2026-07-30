#pragma once

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <memory>
#include <vector>
#include <cstdint>
#include <functional>
#include "logger/logger.hpp"
#include "config/config.hpp"
#include "network/reader.hpp"
#include "network/writer.hpp"
#include "common/packet.hpp"
#include "router/router.hpp"

namespace mysql = boost::mysql;
using boost::asio::ip::tcp;
using boost::asio::steady_timer;
using scalable::common::Packet;

namespace scalable {
namespace server {

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(
        tcp::socket socket, 
        Config& config, 
        std::shared_ptr<Logger> logger,
        mysql::connection_pool& pool,
        std::function<void(std::shared_ptr<Session>)> on_close);

    void start();

    void read_packet();
    void write_packet(std::vector<uint8_t> packet);
    void stop();
    void on_packet(std::vector<uint8_t> packet);

    tcp::endpoint get_remote_endpoint();
    std::string get_remote_address();
    unsigned short get_remote_port();

    boost::asio::any_io_executor get_executor();
    
private:
    void check_idle_timer();

    tcp::socket socket_;
    std::shared_ptr<Logger> logger_;
    Config& config_;
    mysql::connection_pool& connection_pool_;
    std::function<void(std::shared_ptr<Session>)> 
        on_close_;
    bool stopped_=false;

    std::shared_ptr<Reader> reader_;
    std::shared_ptr<Writer> writer_;

    int seconds_idle_ = 300;
    steady_timer idle_timer_;

    Router router_;
};

}
}

#ifndef  SCALABLE_SERVER_CONNECTION_HPP
#define  SCALABLE_SERVER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <cstdint>
#include "logger.hpp"
#include "config.hpp"

using boost::asio::ip::tcp;
using boost::asio::steady_timer;

namespace scalable {
namespace server {

class session : public std::enable_shared_from_this<session>
{
public:
    explicit session(tcp::socket sock, config& cnf, 
        std::shared_ptr<logger> log);
    void start();

private:
    void read_length();
    void read_body();
    void read_data();
    void write_data(size_t length);
    void check_deadline();
    void check_idle();
    void close();

    std::shared_ptr<logger> logger_;
    config& config_;
    int max_message_size_bytes;
    std::vector<char> data;
    tcp::socket socket;
    uint32_t packet_length;
    std::string packet_body;

    steady_timer deadline;
    steady_timer idle_deadline;

    int read_timeout;
    int write_timeout;
    int idle_timeout;
};

}
}

#endif
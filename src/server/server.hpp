#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <atomic>
#include "logger/logger.hpp"
#include "config/config.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Server
{
public: 

explicit Server(boost::asio::io_context& io, 
        Config& cnf, std::shared_ptr<Logger> log);

void start_workers();
void start_accept();
void stop();

private:

void do_accept();
void do_await_stop();
bool add_connection();
void remove_connection();

boost::asio::io_context& io_context_;
tcp::acceptor acceptor;
std::shared_ptr<Logger> logger_;
Config& config_;
std::atomic<size_t> active_connections{0};
size_t max_sessions{0};

};

}
}
#endif
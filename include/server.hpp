#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <atomic>
#include "logger.hpp"
#include "config.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class server
{
public: 

explicit server(boost::asio::io_context& io, 
        config& cnf, logger& log);

void run();
void stop();

private:

void do_accept();
void do_await_stop();
bool add_connection();
void remove_connection();

boost::asio::io_context& io_context;
boost::asio::executor_work_guard
        <boost::asio::io_context::executor_type> work_guard;
tcp::acceptor acceptor;
boost::asio::signal_set signals;
logger& logger_;
config& config_;
std::atomic<size_t> active_connections{0};
size_t max_connections{0};

};

}
}
#endif
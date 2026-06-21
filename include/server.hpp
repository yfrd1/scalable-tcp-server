#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "logger.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class server
{
public: 

explicit server(boost::asio::io_context& io, 
        const std::string& host, const std::string& port,
        size_t thread_pool_size);

void run();

private:

void do_accept();
void do_await_stop();

boost::asio::io_context& io_context;
tcp::acceptor acceptor;
boost::asio::signal_set signals;
size_t max_thread_pool_size;
Logger logger;

};

}
}
#endif
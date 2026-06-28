#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "logger.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class config;

class server
{
public: 

explicit server(boost::asio::io_context& io, 
        std::shared_ptr<config> cnf);

void run();

private:

void do_accept();
void do_await_stop();

boost::asio::io_context& io_context;
tcp::acceptor acceptor;
boost::asio::signal_set signals;
logger logger_;
std::shared_ptr<config> config_;
};

}
}
#endif
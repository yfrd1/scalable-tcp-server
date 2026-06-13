#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class server
{
public: 

explicit server(boost::asio::io_context& io, 
        const std::string& host, const std::string& port);

private:

void do_accept();
void do_await_stop();
        
tcp::acceptor acceptor;
boost::asio::signal_set signals;

};

}
}
#endif
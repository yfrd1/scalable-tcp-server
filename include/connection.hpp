#ifndef  SCALABLE_SERVER_CONNECTION_HPP
#define  SCALABLE_SERVER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include "logger.hpp"
#include "config.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class connection : public std::enable_shared_from_this<connection>
{
public:
    explicit connection(tcp::socket sock, std::shared_ptr<config> cnf);
    void start();

private:
    void read_data();
    void write_data(size_t length);
   
    std::shared_ptr<config> config_;
    logger logger_;
    std::array<char, 512> data;
    tcp::socket socket;
};

}
}

#endif
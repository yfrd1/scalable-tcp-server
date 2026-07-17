#include <boost/asio.hpp>
#include "writer.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {


    Writer::Writer(tcp::socket& socket) :
    socket_(socket)
    {

    }
}
}
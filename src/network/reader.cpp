#include <boost/asio.hpp>
#include "reader.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {


    Reader::Reader(tcp::socket& socket) :
    socket_(socket)
    {

    }

}
}
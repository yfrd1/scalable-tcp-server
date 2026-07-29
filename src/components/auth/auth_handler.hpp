#pragma once

#include <boost/mysql.hpp>

namespace scalable::common{ class Packet; }
namespace mysql = boost::mysql;
using scalable::common::Packet;

namespace scalable {
namespace server {

class Session;

class AuthHandler
{
public:

    AuthHandler() = default;

    AuthHandler(const AuthHandler&) = delete;
    AuthHandler& operator=(const AuthHandler&) = delete;

    AuthHandler(AuthHandler&&) = delete;
    AuthHandler& operator=(AuthHandler&&) = delete;

    void handle(Session& session, Packet& packet, 
        mysql::connection_pool& connection_pool);    
 
private:

};

}
}
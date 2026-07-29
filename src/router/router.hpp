#pragma once

#include <boost/mysql.hpp>

#include "components/auth/auth_handler.hpp"

#include "common/packet_enums.hpp"
#include "session/session.hpp"

using scalable::common::PacketType;

namespace scalable {
namespace server {

class Router
{
public:
    Router() = default;

    Router(const Router&) = delete;
    Router& operator = (const Router&) = delete;
    
    Router(Router&&) = delete;
    Router& operator = (Router&&) = delete;

    void route(Session& session, Packet& packet, mysql::connection_pool& pool);

private:
    AuthHandler auth_handler_;
    //AccountHandler account_handler;
    //ChatHandler chat_handler_;
    //FileHandler file_handler_;
    //StreamHandler stream_handler_;
};

}
}
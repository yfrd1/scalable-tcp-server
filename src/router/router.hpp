#pragma once

#include "handlers/auth_handler.hpp"
#include "handlers/account_handler.hpp"
#include "handlers/chat_handler.hpp"
#include "handlers/file_handler.hpp"
#include "handlers/stream_handler.hpp"

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

    void route(Session& session, Packet& packet);

private:
    AuthHandler auth_handler_;
    AccountHandler account_handler;
    //ChatHandler chat_handler_;
    //FileHandler file_handler_;
    //StreamHandler stream_handler_;
};

}
}
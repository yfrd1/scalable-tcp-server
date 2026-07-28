#pragma once

#include "session/session.hpp"
#include "common/packet.hpp"

namespace scalable {
namespace server {

    class AuthService
    {
    public:
        AuthService() = default;

        void loginService(Session& session, Packet& packet);
        void registerService(Session& session, Packet& packet);
        

    };
}
}
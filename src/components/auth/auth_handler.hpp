#pragma once

#include "session/session.hpp"
#include "common/packet.hpp"

namespace scalable {
namespace server {

class AuthHandler
{
public:
    enum class AuthAction : uint8_t
    {
        Login = 1,
        Register = 2
    };

    AuthHandler() = default;

    AuthHandler(const AuthHandler&) = delete;
    AuthHandler& operator=(const AuthHandler&) = delete;

    AuthHandler(AuthHandler&&) = delete;
    AuthHandler& operator=(AuthHandler&&) = delete;

    void handle(Session& session, Packet& packet);    
 
private:

};

}
}
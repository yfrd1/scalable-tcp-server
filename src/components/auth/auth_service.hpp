#pragma once

#include "common/auth/auth_types.hpp"
using scalable::common::LoginRequest;

namespace scalable {
namespace server {

class Session;

class AuthService
{
public:
    AuthService() = default;

    void login(Session& session, LoginRequest& request);
    

};


}
}
#pragma once

#include <boost/mysql.hpp>
#include <boost/asio/awaitable.hpp>
#include "common/auth/auth_types.hpp"
using scalable::common::LoginRequest;

namespace mysql = boost::mysql;

namespace scalable {
namespace server {

class Session;

class AuthService
{
public:
    AuthService() = default;

    boost::asio::awaitable<void>
    login(
      std::shared_ptr<Session> session, LoginRequest request,
      mysql::connection_pool& connection_pool);

};


}
}
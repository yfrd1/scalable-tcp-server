#pragma once

#include <boost/mysql.hpp>
#include <boost/asio.hpp>

#include "common/auth/auth_types.hpp"

namespace asio = boost::asio;
namespace mysql = boost::mysql;

using scalable::common::LoginRequest;

namespace scalable {
namespace server {


class AuthRepository
{
public:
    explicit AuthRepository(mysql::connection_pool& connection_pool);
    
    asio::awaitable<bool> selectUser(mysql::pooled_connection& conn, LoginRequest& request);
    
    asio::awaitable<bool> insertUser(
      mysql::pooled_connection& conn, 
      const std::string& session_id, 
      const std::string& user_id, 
      const std::string& ip);

    asio::awaitable<void> updateUser(mysql::pooled_connection& conn, LoginRequest& request);
    

private:
    mysql::connection_pool& connection_pool_;
};


}
}
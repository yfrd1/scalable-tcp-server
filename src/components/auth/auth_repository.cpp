#include "components/auth/auth_repository.hpp"

#include <boost/asio.hpp>
#include <boost/mysql.hpp>

#include "common/auth/auth_types.hpp"

namespace asio = boost::asio;
namespace mysql = boost::mysql;

using scalable::common::LoginRequest;

namespace scalable {
namespace server {



    AuthRepository::AuthRepository(mysql::connection_pool& connection_pool):
        connection_pool_(connection_pool){}
    
    asio::awaitable<bool> AuthRepository::selectUser(
      mysql::pooled_connection& conn, LoginRequest& request)
    {

            mysql::statement stmt = 
                co_await conn->async_prepare_statement(
                    "SELECT * FROM users WHERE" 
                        " (user_id=? AND password_hash=? AND is_active=1) LIMIT 1");

            mysql::results result;
            co_await conn->async_execute(
                stmt.bind(request.user_id, request.password), 
                result);

        co_return !result.rows().empty();
    }

    asio::awaitable<bool> AuthRepository::insertUser(
      mysql::pooled_connection& conn, 
      const std::string& session_id, 
      const std::string& user_id, 
      const std::string& ip)
    {

        mysql::statement smtm_insert = co_await 
        conn->async_prepare_statement(
          "INSERT INTO login_sessions "
          "(session_id, user_id, created_at, expires_at, last_access_at, ip_address) VALUES "
          "(?, ?, NOW(), DATE_ADD(NOW(), INTERVAL 7 DAY), NOW(), ?)");

        
        mysql::results result;
        co_await conn->async_execute(
                    smtm_insert.bind(
                        session_id, user_id, ip),
                    result);

        co_return result.last_insert_id()>0;
    }

    asio::awaitable<void> AuthRepository::updateUser(
        mysql::pooled_connection& conn, LoginRequest& request)
    {

        mysql::statement smtm_update = co_await 
        conn->async_prepare_statement(
          "UPDDATE users set last_login_at=NOW() "
          "WHERE user_id=? and is_active=1");
        
        mysql::results result;
        co_await conn->async_execute(
                    smtm_update.bind(request.user_id),
                    result);

        co_return;
    }
    


}
}
#include "components/auth/auth_service.hpp"

#include <memory>
#include <boost/mysql.hpp>

#include "session/session.hpp"
#include "common/packet.hpp"
#include "common/utils/validation.hpp"
#include "common/auth/auth_types.hpp"

namespace mysql = boost::mysql;

using scalable::common::LoginRequest;
using scalable::common::Validation;

namespace scalable {
namespace server {

    boost::asio::awaitable<void>
    AuthService::login(
      std::shared_ptr<Session> session, LoginRequest request,
      mysql::connection_pool& connection_pool)
    {

        if(Validation::isValidUserId(request.user_id))
        {

        }
        else if(Validation::isValidUserIdLength(request.user_id))
        {
            
        }
        else if(Validation::isValidPasswordLength(request.password))
        {

        }
        else
        {
            mysql::pooled_connection conn = 
                co_await connection_pool.async_get_connection();           
            
            mysql::statement stmt = 
                co_await conn->async_prepare_statement(
                    "SELECT * FROM users WHERE" 
                        " (user_id=? AND password_hash=? AND is_active=1) LIMIT 1");

            mysql::results result;
            co_await conn->async_execute(
                stmt.bind(request.user_id, request.password), 
                result);

            if(!result.rows().empty())
            {

                mysql::statement smtm_insert = co_await 
                    conn->async_prepare_statement(
                        "INSERT INTO login_sessions "
                        "(session_id, user_id, created_at, expires_at, last_access_at, ip_address) VALUES "
                        "(?, ?, NOW(), DATE_ADD(NOW(), INTERVAL 7 DAY), NOW(), ?)");

                auto ip = session->get_remote_address();

                //Generate the session ID using a cryptographically secure random generator
                co_await conn->async_execute(
                    smtm_insert.bind(
                        "session_id", request.user_id, ip),
                    result);

                if(result.last_insert_id()>0)
                {
                    mysql::statement stmt_update = co_await
                        conn->async_prepare_statement(
                            "UPDATE users SET last_login_at=NOW() "
                            "WHERE user_id=?");

                    co_await conn->async_execute(
                        stmt_update.bind(request.user_id),
                        result);

                    
                }
                else
                {

                }
            }
            else
            {

            }
        }

        co_return;
    }
    
}
}
#include "components/auth/auth_service.hpp"

#include <memory>
#include <boost/mysql.hpp>

#include "components/auth/auth_repository.hpp"
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
            AuthRepository repo(connection_pool);
            
            mysql::pooled_connection conn = 
                co_await connection_pool.async_get_connection();           
            
            bool finded = co_await repo.selectUser(conn, request);
            if(finded)
            {
                auto ip = session->get_remote_address();

                //Generate the session ID using a cryptographically secure random generator
                bool inserted = co_await repo.insertUser(conn, "session_id", request.user_id, ip);
                if(inserted)
                {
                    repo.updateUser(conn, request);
                }
                else
                {

                }
            }
            else
            {

            }

            co_return;
        }
    }
}
}
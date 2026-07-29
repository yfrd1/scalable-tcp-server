
#include <memory>
#include <boost/mysql.hpp>
#include "components/auth/auth_service.hpp"
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

        if(Validation::isValidUsername(request.username))
        {
            
        }
        else if(Validation::isValidUsernameLength(request.username))
        {
            
        }
        else if(Validation::isValidPasswordLength(request.password))
        {

        }
        else
        {
            
        }

        co_return;
    }
    
}
}
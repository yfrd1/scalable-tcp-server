#include "components/auth/auth_handler.hpp"

#include <boost/mysql.hpp>
#include <boost/asio.hpp>

#include "common/packet.hpp"
#include "common/sub_header_type.hpp"
#include "common/auth/auth_validator.hpp"
#include "session/session.hpp"
#include "components/auth/auth_service.hpp"

namespace mysql = boost::mysql; 

using scalable::common::SubHeaderType;
using scalable::common::auth::AuthValidator;
using scalable::common::auth::AuthAction;
using scalable::common::auth::LoginRequest;
using scalable::common::auth::deserialize_login_request;
using scalable::common::Packet;


namespace scalable {
namespace server {


    void AuthHandler::handle(Session& session, Packet& packet,
        mysql::connection_pool& connection_pool)
    {
        auto value = packet.sub_header_get_uint8(SubHeaderType::Action);
        if(!value)
        {
            return;
        }
        
        AuthAction actionType = AuthValidator::action_from_byte(*value);
        if(actionType == AuthAction::Unknown)
            return;

        if(actionType == AuthAction::Login)
        {
            LoginRequest request =
                deserialize_login_request(packet.payload(), packet.payload_size());

            AuthService service;
            auto self = session.shared_from_this();
            boost::asio::co_spawn(
                session.get_executor(),
                service.login(
                    self, request, connection_pool
                ),
                boost::asio::detached);

        }

    }

}
}
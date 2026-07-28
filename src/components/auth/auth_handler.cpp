#include "auth_handler.hpp"
#include "auth_types.hpp"
#include "common/sub_header_type.hpp"
#include "common/auth/auth_validator.hpp"

using scalable::common::SubHeaderType;
using scalable::common::AuthValidator;
using scalable::common::AuthAction;
using scalable::common::LoginRequest;
namespace deserialize = scalable::common::deserialize;


namespace scalable {
namespace server {


    void AuthHandler::handle(Session& session, Packet& packet)
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
                deserialize::login_request(packet.payload(), packet.payload_size());
        }

    }

}
}
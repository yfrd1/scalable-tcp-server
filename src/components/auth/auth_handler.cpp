#include "auth_handler.hpp"
#include "common/sub_header_type.hpp"
#include "common/auth/auth_validator.hpp"

using scalable::common::SubHeaderType;
using scalable::common::AuthValidator;

namespace scalable {
namespace server {


    void AuthHandler::handle(Session& session, Packet& packet)
    {
        auto value = packet.sub_header_get_uint8(SubHeaderType::Action);
        if(!value)
        {
            return;
        }
        
        if(!AuthValidator::validateAction(*value))
            return;

    }

}
}
#include "auth_handler.hpp"
#include "common/sub_header_type.hpp"
#include "common/auth_data_types.hpp"

using scalable::common::SubHeaderType;

namespace scalable {
namespace server {


    void AuthHandler::handle(Session& session, Packet& packet)
    {
        auto value = packet.sub_header_get_uint8(SubHeaderType::Action);
        if(!value)
        {
            return;
        }

        
        
    }

}
}
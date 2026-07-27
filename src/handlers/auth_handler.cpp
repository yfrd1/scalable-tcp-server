#include "auth_handler.hpp"
#include "common/sub_header_type.hpp"

using scalable::common::SubHeaderType;

namespace scalable {
namespace server {


    void AuthHandler::handle(Session& session, Packet& packet)
    {
        const auto& value = packet.sub_header_value(SubHeaderType::Action);
        if(value.empty())
        {
            return;
        }

        
    }

}
}
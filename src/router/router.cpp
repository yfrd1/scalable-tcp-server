#include "router/router.hpp"
#include "common/packet_enums.hpp"
#include "common/packet.hpp"

using scalable::common::PacketType;

namespace scalable {
namespace server {

    void Router::route(Session& session, Packet& packet,
        mysql::connection_pool& connection_pool)
    {
        
        switch (packet.packet_type())
        {
        case PacketType::Auth:
            auth_handler_.handle(session, packet, connection_pool);
            break;

        default:
            break;
        }
    }
  

}
}
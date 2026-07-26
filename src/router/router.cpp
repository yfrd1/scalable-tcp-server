#include "router/router.hpp"
#include "common/packet_enums.hpp"
#include "common/packet.hpp"

using scalable::common::PacketType;

namespace scalable {
namespace server {

    void Router::route(Session& session, Packet& packet)
    {
        switch (packet.packet_type())
        {
        case PacketType::Auth:
            
        case PacketType::Account:
        default:
            break;
        }
    }
  

}
}
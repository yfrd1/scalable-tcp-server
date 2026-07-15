#include <utility>
#include <vector>
#include "packet.hpp"

namespace scalable {
namespace common {
    

    Packet::Packet(const PacketHeader& header, 
        std::vector<uint8_t> metadata,
        std::vector<uint8_t> payload) :
        header_(header),
        metadata_(std::move(metadata)),
        payload_(std::move(payload))
    {

    }

    PacketHeader& Packet::get_header()
    {
        return header_;
    }

    const PacketHeader& Packet::get_header() const
    {
        return header_;
    }

    std::vector<uint8_t>& Packet::get_metadata()
    {
        return metadata_;
    }

    const std::vector<uint8_t>& Packet::get_metadata() const
    {
        return metadata_;
    }
    
    std::vector<uint8_t>& Packet::get_payload()
    {
        return payload_;
    }

    const std::vector<uint8_t>& Packet::get_payload() const
    {
        return payload_;
    }    


}
}
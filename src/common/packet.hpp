#pragma once

#include <string>
#include <vector>
#include "packet_type.hpp"
#include "packet_header.hpp"

namespace scalable {
namespace common {
    
class Packet
{
public:
    Packet() = default;
    Packet(const PacketHeader& header, 
        std::vector<uint8_t> metadata,
        std::vector<uint8_t> payload);

    PacketHeader& get_header();
    const PacketHeader& get_header() const;

    std::vector<uint8_t>& get_metadata();
    const std::vector<uint8_t>& get_metadata() const;
    
    std::vector<uint8_t>& get_payload();
    const std::vector<uint8_t>& get_payload() const;    

    
private:
    PacketHeader header_;
    std::vector<uint8_t> metadata_;
    std::vector<uint8_t> payload_;
};


}
}
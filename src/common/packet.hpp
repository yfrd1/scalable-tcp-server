#pragma once

/*

Packet format:

+----------------+----------+---------------+--------------+
| packet_size    | version  | packet_type   | header_size  |
| 4 bytes        | 1 byte   | 1 byte        | 2 bytes      |
+----------------+----------+---------------+--------------+
| flags          | sequence_id                             |
| 2 bytes        | 4 bytes                                 |
+----------------------------------------------------------+
|                                                          |
| headers (variable)                                       |
|                                                          |
+----------------------------------------------------------+
|                                                          |
| payload (variable)                                       |
|                                                          |
+----------------------------------------------------------+

*/


#include <cstdint>
#include <vector>
#include "common/packet_enums.hpp"

namespace scalable {
namespace common {

struct OutgoingPacket;

class Packet
{
public:
    explicit Packet(std::vector<uint8_t>&& bytes);

    static OutgoingPacket create(
        PacketVersion version,
        PacketType type,
        PacketFlags flags,
        uint32_t sequence_id,
        std::vector<uint8_t>&& headers,
        std::vector<uint8_t>&& payload
    );

    uint32_t packet_size() const;
    PacketVersion version() const;
    PacketType packet_type() const;
    uint16_t header_size() const;
    PacketFlags flags() const;
    uint32_t sequence_id() const;
    uint32_t payload_size() const;

    const uint8_t* headers() const;
    const uint8_t* payload() const;
    
    static PacketType packet_type_from_byte(uint8_t type);
    static uint8_t packet_type_to_byte(PacketType type);

    static PacketVersion packet_version_from_byte(uint8_t version);
    static uint8_t packet_version_to_byte(PacketVersion version);

    static PacketFlags packet_flags_from_bytes(uint16_t flags);
    static uint16_t packet_flags_to_bytes(PacketFlags flags);

    static constexpr uint32_t PACKET_LENGTH = 4; // Use in Reader

    static constexpr uint16_t MAX_HEADER_SIZE = 1024;
    static constexpr uint32_t MAX_PAYLOAD_SIZE = 1024 * 1024;
    static constexpr uint32_t MAX_PACKET_SIZE =
        MAX_HEADER_SIZE + MAX_PAYLOAD_SIZE;

private:
   
    uint32_t packet_size_;
    PacketVersion version_;
    PacketType packet_type_;
    // header_size_ represents the size of the message header,
    // not the total size of packet fields.
    uint16_t header_size_;
    PacketFlags flags_;
    uint32_t sequence_id_;

    constexpr static size_t offset_packet_size_     = 0;
    constexpr static size_t offset_version_         = 4;
    constexpr static size_t offset_packet_type_     = 5;
    constexpr static size_t offset_header_size_     = 6;
    constexpr static size_t offset_flags_           = 8;
    constexpr static size_t offset_sequence_id_     = 10;
    constexpr static size_t offset_headers_         = 14;
    size_t offset_payload_;  // 14 + header_size_ at run time

    uint16_t headers_size_;
    uint32_t payload_size_;
    
    std::vector<uint8_t> buffer_;

};


}
}
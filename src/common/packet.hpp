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

Notes:

    1. All numeric fields must be serialized using network byte order
       (Big Endian) to ensure compatibility between different systems.

    2. packet_size does NOT include its own 4 bytes.
       It represents the size of the remaining packet data after packet_size.

       This allows the receiver to:
           - Read the first 4 bytes.
           - Extract packet_size.
           - Read exactly packet_size more bytes.

    3. Do not send the raw Packet struct directly.
       Serialize and deserialize each field explicitly to avoid
       padding, alignment, and endian issues.

*/


#include <cstdint>
#include <vector>
#include "common/packet_type.hpp"

namespace scalable {
namespace common {
    
class Packet
{
public:
    explicit Packet(std::vector<uint8_t>&& bytes);

    static Packet create(
        uint8_t version,
        PacketType packet_type,
        uint16_t flags,
        uint32_t sequence_id,
        std::vector<uint8_t>&& headers,
        std::vector<uint8_t>&& payload
    );

    uint32_t packet_size() const;
    uint8_t version() const;
    PacketType packet_type() const;
    uint16_t header_size() const;
    uint16_t flags() const;
    uint32_t sequence_id() const;
    uint32_t payload_size() const;
    uint32_t buffer_size() const;

    const uint8_t* headers() const;
    const uint8_t* payload() const;
    const uint8_t* buffer() const;

private:
    /*
    void packet_size(uint32_t size);
    void version(uint8_t version);
    void packet_type(PacketType type);
    void header_size(uint16_t size);
    void flags(uint16_t flags);
    void sequence_id(uint32_t id);
    */
   
    uint32_t packet_size_;
    uint8_t version_;
    PacketType packet_type_;
    uint16_t header_size_;
    uint16_t flags_;
    uint32_t sequence_id_;

    uint16_t headers_offset_;
    uint16_t headers_size_;
    uint32_t payload_offset_;
    uint32_t payload_size_;
    
    std::vector<uint8_t> buffer_;
};


}
}
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


namespace scalable {
namespace common {
    
class Packet
{
public:

    
    
private:
    uint32_t packet_size_;
    uint8_t version_;
    uint8_t packet_type_;
    uint16_t header_size_;
    uint16_t flags_;
    uint32_t sequence_id_;

    size_t headers_offset_;
    size_t payload_offset_;

    std::vector<uint8_t> buffer_;
};


}
}
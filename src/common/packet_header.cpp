#include "packet_type.hpp"
#include "packet_header.hpp"

namespace scalable {
namespace common {

    PacketHeader::PacketHeader(PacketType type, 
      uint32_t metadata_size, uint64_t payload_size) :
      type_(type), 
      metadata_size_(metadata_size),
      payload_size_(payload_size)
    {

    }

    PacketType PacketHeader::get_type() const
    {
        return type_;
    }

    uint32_t PacketHeader::get_metadata_size() const
    {
        return metadata_size_;
    }

    uint64_t PacketHeader::get_payload_size() const
    {
        return payload_size_;
    }

    void PacketHeader::set_type(PacketType type)
    {
        type_=type;
    }

    void PacketHeader::set_metadata_size(uint32_t size)
    {
        metadata_size_=size;
    }

    void PacketHeader::set_payload_size(uint64_t size)
    {
        payload_size_=size;
    }

}
}
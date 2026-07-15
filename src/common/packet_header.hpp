#include <string>
#include <cstdint>
#include <vector>
#include "packet_type.hpp"

namespace scalable {
namespace common {

class PacketHeader
{
public:
    PacketHeader() = default;
    explicit PacketHeader(PacketType type, 
      uint32_t metadata_size, uint64_t payload_size);

    PacketType get_type() const;
    uint32_t get_metadata_size() const;
    uint64_t get_payload_size() const;

    void set_type(PacketType type);
    void set_metadata_size(uint32_t size);
    void set_payload_size(uint64_t size);

private:
    PacketType type_{ PacketType::Text };
    uint32_t metadata_size_;
    uint64_t payload_size_;
};

}
}
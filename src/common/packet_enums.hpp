#pragma once

#include <cstdint>

namespace scalable {
namespace common {



enum class PacketVersion : uint8_t
{
    Unknown = 0,
    v1 = 1
};

enum class PacketType : uint8_t 
{
    Unknown = 0,
    Text = 1,
    File = 2,
    Video = 3
};

enum class PacketFlags : uint16_t
{
    None = 0,
    //Compressed = 1 << 0,
    //Encrypted = 1 << 1
};

}
}
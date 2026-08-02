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
    Unknown =           0,
    Auth    =           1,
    Account =           2,
    Text    =           3,
    File    =           4,
    Video   =           5
};

enum class PacketFlags : uint16_t
{
    None = 0,
    //Compressed = 1 << 0,
    //Encrypted = 1 << 1
};

}
}
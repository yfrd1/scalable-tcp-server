#pragma once

#include <cstdint>

namespace scalable {
namespace common {


enum class PacketType : uint16_t 
{
    Unknown = 0,
    Text = 1,
    File = 2,
    Video = 3
};


}
}
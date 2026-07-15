#pragma once

#include <cstdint>

namespace scalabale::server {


enum class PacketType : uint16_t 
{
    Text = 1,
    File = 2,
    Video = 3
};


}
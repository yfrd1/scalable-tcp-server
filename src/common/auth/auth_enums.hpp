#pragma once

#include <cstdint>

namespace scalable {
namespace common {
  
    enum class AuthAction : uint8_t
    {
        Unknown     =     0,

        Login       =     10,
        Register    =     11
        
    };

}
}
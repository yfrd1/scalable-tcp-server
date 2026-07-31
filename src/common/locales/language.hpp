#pragma once

#include <cstdint>
#include <string_view>
#include <array>

namespace scalable {
namespace common {

    enum class Language : uint8_t
    {
        en = 1
    };

    inline std::array<Language, 1> languages {
        Language::en
    };

}
}
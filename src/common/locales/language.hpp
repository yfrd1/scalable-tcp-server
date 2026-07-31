#pragma once

#include <cstdint>
#include <string>
#include <map>

namespace scalable {
namespace common {

    enum class Language : uint8_t
    {
        en = 1
    };

    inline std::map<Language, std::string> languages {
        {Language::en, "en"}
    };

}
}
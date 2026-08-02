#pragma once

#include <cstdint>
#include <string>
#include <map>

namespace scalable::common::locales {

    enum class Language : uint8_t
    {
        en = 1
    };

    inline std::map<Language, std::string> languages {
        {Language::en, "en"}
    };

}
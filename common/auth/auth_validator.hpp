#pragma once

#include <cstdint>

#include "auth_types.hpp"

namespace scalable::common::auth {

class AuthValidator
{
public:
    AuthValidator() = default;

    static bool validateAction(uint8_t action);
    static AuthAction action_from_byte(uint8_t action);
};

}
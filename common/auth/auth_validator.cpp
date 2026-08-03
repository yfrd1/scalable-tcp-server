#include "common/auth/auth_validator.hpp"

#include <cstdint>

#include "common/auth/auth_types.hpp"

using scalable::common::auth::AuthAction;

namespace scalable::common::auth {

    bool AuthValidator::validateAction(uint8_t action)
    {
        return action_from_byte(action)!=AuthAction::Unknown;
    }

    AuthAction AuthValidator::action_from_byte(uint8_t action)
    {
        switch (action)
        {
        case 10:
            return AuthAction::Login;
        case 11:
            return AuthAction::Register;

        default:
            return AuthAction::Unknown;
        }
    }

}

#include <cstdint>
#include "auth_validator.hpp"
#include "auth_types.hpp"

namespace scalable {
namespace common {

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
}
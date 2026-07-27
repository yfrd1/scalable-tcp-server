#include "auth_validator.hpp"
#include "auth_enums.hpp"

namespace scalable {
namespace common {

    bool AuthValidator::validateAction(AuthAction action) const
    {
        return action==AuthAction::Login || 
            action==AuthAction::Register;
    }


}
}
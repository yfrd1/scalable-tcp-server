#pragma once

#include "auth_enums.hpp"

namespace scalable {
namespace common {

class AuthValidator
{
public:
    AuthValidator() = default;

    bool validateAction(AuthAction action) const;
};

}
}
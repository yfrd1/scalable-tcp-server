#pragma once

#include <string_view>

namespace scalable {
namespace common {

class Validation
{
public:
    static bool isValidUsername(std::string_view username);
    static bool isValidUsernameLength(std::string_view username);
    static bool isValidEmail(std::string_view email);
    static bool isValidPasswordLength(std::string_view password);

};

}
}
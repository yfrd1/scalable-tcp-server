#pragma once

#include <string_view>

namespace scalable {
namespace common {

class Validation
{
public:
    static bool isValidUserId(std::string_view user_id);
    static bool isValidUserIdLength(std::string_view user_id);
    static bool isValidEmail(std::string_view email);
    static bool isValidPasswordLength(std::string_view password);

};

}
}
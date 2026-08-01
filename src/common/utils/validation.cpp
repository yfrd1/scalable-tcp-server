#include "common/utils/validation.hpp"

#include <string_view>
#include <regex>

namespace scalable {
namespace common {

    bool Validation::isValidUserId(std::string_view user_id)
    {
        std::regex pattern(R"(^[a-zA-Z0-9_\.-]+$)");
        return std::regex_match(user_id.begin(), user_id.end(),
            pattern);
    }

    bool Validation::isValidUserIdLength(std::string_view user_id)
    {
        return user_id.length() < 4;
    }   

    bool Validation::isValidEmail(std::string_view email)
    {
        std::regex pattern (R"(^[a-zA-z0-9_\.-]+@[a-zA-Z0-9_-]+\.[a-zA-Z0-9]{2,3}$)");
        return std::regex_match(email.begin(), email.end(), pattern);
    }

    bool Validation::isValidPasswordLength(std::string_view password)
    {
        return password.length() < 6;
    }

}
}
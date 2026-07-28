#include <string_view>
#include <regex>
#include "validation.hpp"

namespace scalable {
namespace common {

    bool Validation::isValidUsername(std::string_view username)
    {
        std::regex pattern(R"(^[a-zA-Z0-9_\.-]+$)");
        return std::regex_match(username.begin(), username.end(),
            pattern);
    }

    bool Validation::isValidUsernameLength(std::string_view username)
    {
        return username.length() < 4;
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
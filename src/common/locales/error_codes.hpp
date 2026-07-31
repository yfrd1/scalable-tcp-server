#pragma once

#include <cstdint>

namespace scalable {
namespace common {

enum class ErrorCodes : uint16_t
{
    Success                     = 1,

    InvalidUserId               = 101,
    InvalidUserIdLength         = 102,
    InvalidPasswordLength       = 103,

    DatabaseUserNotFound        = 201,
    DatabaseInsertFailed        = 202

};

}
}
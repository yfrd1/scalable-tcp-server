#pragma once

#include <cstdint>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "common/auth/error_code.hpp"

namespace scalable::common::auth {
  
    enum class AuthAction : uint8_t
    {
        Unknown             = 0,

        Login               = 10,
        LoginResponse       = 11,

        Register            = 20,
        RegisterResponse    = 21        
    };
    
    enum class AuthStatus : uint8_t
    {
        Error               = 1,
        Success             = 2
    };

    struct LoginRequest
    {
        std::string user_id;
        std::string password;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & user_id;
            ar & password;
        }
    };

    struct LoginResponse
    {
        AuthStatus status;
        ErrorCode error_code;
        std::string session_id;
        
        template<typename Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & status;
            ar & error_code;
            ar & session_id;
        }
    };

    LoginRequest deserialize_login_request(
        const uint8_t* body, uint32_t size);

    
}

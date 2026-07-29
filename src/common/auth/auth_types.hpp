#pragma once

#include <cstdint>
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace scalable {
namespace common {
  
    enum class AuthAction : uint8_t
    {
        Unknown     =     0,

        Login       =     10,
        Register    =     11
        
    };
    

    struct LoginRequest
    {
        std::string username;
        std::string password;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & username;
            ar & password;
        }
    };

    LoginRequest deserialize_login_request(
        const uint8_t* body, uint32_t size);

    
}
}
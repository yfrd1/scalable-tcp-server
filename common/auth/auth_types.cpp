#include "common/auth/auth_types.hpp"

#include <cstdint>
#include <string>
#include <sstream>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace scalable::common::auth {
 
        LoginRequest deserialize_login_request(
            const uint8_t* body, uint32_t size)
        {
            // TODO: Optimize this later to avoid unnecessary data copies.
            std::string data(reinterpret_cast<const char*>(body), size);
            std::istringstream stream(data);
            boost::archive::binary_iarchive arch(stream);

            LoginRequest request;
            arch >> request;

            return request;
        }
    
}

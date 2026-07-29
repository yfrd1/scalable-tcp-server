#include <cstdint>
#include <string>
#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "common/auth/auth_types.hpp"

namespace scalable {
namespace common {
 
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
}
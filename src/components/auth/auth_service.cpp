
#include "components/auth/auth_service.hpp"
#include "session/session.hpp"
#include "common/packet.hpp"
#include "common/utils/validation.hpp"
#include "common/auth/auth_types.hpp"

using scalable::common::LoginRequest;
using scalable::common::Validation;

namespace scalable {
namespace server {

    void AuthService::login(
      Session& session, LoginRequest& request)
    {

        if(Validation::isValidUsername(request.username))
        {

        }
        else if(Validation::isValidUsernameLength(request.username))
        {

        }
        else if(Validation::isValidPasswordLength(request.password))
        {

        }
        else
        {
            
        }
    }
    
 
    
}
}
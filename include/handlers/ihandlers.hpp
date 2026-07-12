
#include <string>

namespace scalable {
namespace server {

class connection;

class IHandler
{
public:
    virtual ~IHandler() = default;

    virtual void handle(
        std::string message,
        connection& session) = 0;
};

}
}
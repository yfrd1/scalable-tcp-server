#pragma once

#include <cstdint>
#include <string>


namespace scalable {
namespace server {

struct packet
{
    uint32_t length{};
    std::string payload;
};

}
}
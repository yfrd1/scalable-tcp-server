#pragma once

#include <cstdint>
#include <string>

struct packet
{
    uint32_t length{};
    std::string payload;
};
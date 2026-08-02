cmake_minimum_required(VERSION 3.16)

project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SERVER_SOURCES src/*.cpp)
file(GLOB_RECURSE COMMON_SOURCES common/*.cpp)

add_executable(server
    ${SERVER_SOURCES} ${COMMON_SOURCES}
)

target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Boost REQUIRED COMPONENTS json charconv serialization)
find_package(OpenSSL REQUIRED)

target_link_libraries(server PRIVATE
    Boost::json
    Boost::charconv
    Boost::serialization
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
)
cmake_minimum_required(VERSION 3.16)

project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(server
    ${SOURCES}
)

target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Boost REQUIRED COMPONENTS json)

target_link_libraries(server PRIVATE
    Boost::json
    pthread
)